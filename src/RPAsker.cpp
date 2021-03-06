//
// Created by Gerard Canal <gcanal@iri.upc.edu> on 31/10/18.
//

#include <rosplan_interface_turtlebot/RPAsker.h>

#include "rosplan_interface_turtlebot/RPAsker.h"

namespace  KCL_rosplan {

    RPAsker::RPAsker(ros::NodeHandle& nh) : nh_(nh) {
        // listen for action dispatch
        std::string adt = "default_dispatch_topic";
        nh.getParam("action_dispatch_topic", adt);
        dispatcher = nh.subscribe(adt, 1000, &KCL_rosplan::RPAsker::dispatchCallback, this);
        nh.param("turtlebot_name", robot_name, std::string("kenny"));

        // create publishers
        std::string aft = "default_feedback_topic";
        nh.getParam("action_feedback_topic", aft);
        action_feedback_pub = nh.advertise<rosplan_dispatch_msgs::ActionFeedback>(aft, 10, true);
        tts_pub = nh.advertise<std_msgs::String>("/speech", 1, true);

        // knowledge interface
        update_knowledge_client = nh.serviceClient<rosplan_knowledge_msgs::KnowledgeUpdateService>("/kcl_rosplan/update_knowledge_base");
    }

    void RPAsker::speak(const std::string &s) {
        std_msgs::String msg;
        msg.data = s;
        tts_pub.publish(msg);
    }

    void RPAsker::dispatchCallback(const rosplan_dispatch_msgs::ActionDispatch::ConstPtr &msg) {
        std::string to_say, predicate;
        bool correct_action = false;

        if (msg->name == "ask_load") {
            correct_action  = true;
            predicate = "asked_load";
            to_say = "Please, would you be kind enough to fetch the printed papers and put them on top of me?";
        }
        else if (msg->name == "ask_unload") {
            correct_action = true;
            predicate = "asked_unload";
            to_say = "Your printings are ready! Can you get the papers from me?";
        }

        if (correct_action) {
            ROS_INFO("(Asker) Received action %s", msg->name.c_str());
            if (msg->parameters[0].value != robot_name) return;

            // publish feedback (enabled)
            rosplan_dispatch_msgs::ActionFeedback fb;
            fb.action_id = msg->action_id;
            fb.status = rosplan_dispatch_msgs::ActionFeedback::ACTION_ENABLED;
            action_feedback_pub.publish(fb);

            speak(to_say);
            ros::Duration(0.075*to_say.length()).sleep(); // Duration proportional to the said speech

            // add predicate
            rosplan_knowledge_msgs::KnowledgeUpdateService updatePredSrv;
            updatePredSrv.request.update_type = rosplan_knowledge_msgs::KnowledgeUpdateService::Request::ADD_KNOWLEDGE;
            updatePredSrv.request.knowledge.knowledge_type = rosplan_knowledge_msgs::KnowledgeItem::FACT;
            updatePredSrv.request.knowledge.attribute_name = predicate;
            diagnostic_msgs::KeyValue pair;
            pair.key = "r";
            pair.value = robot_name;
            updatePredSrv.request.knowledge.values.push_back(pair);
            update_knowledge_client.call(updatePredSrv);

            // publish feedback (achieved)
            fb.action_id = msg->action_id;
            fb.status = rosplan_dispatch_msgs::ActionFeedback::ACTION_SUCCEEDED_TO_GOAL_STATE;
            action_feedback_pub.publish(fb);
        }
    }
}


/*-------------*/
/* Main method */
/*-------------*/

int main(int argc, char **argv) {

    ros::init(argc, argv, "rosplan_asker_interface");
    ros::NodeHandle nh("~");

    // create PDDL action subscriber
    KCL_rosplan::RPAsker rpa(nh);
    ROS_INFO("KCL: (Asker) Ready to receive");

    ros::spin();
    return 0;
}
