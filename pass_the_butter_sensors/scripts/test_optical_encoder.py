#!/usr/bin/env python
import rospy

from std_msgs.msg import Int32

from grove.grove_optical_rotary_encoder import GroveOpticalRotaryEncoder
from grove.helper import SlotHelper

if __name__ == "__main__":
    rospy.init_node("optical_ecoder_test")

    sh = SlotHelper(SlotHelper.GPIO)
    pin = sh.argv2pin()
    encoder = GroveOpticalRotaryEncoder(pin)
    encoder_pub = rospy.Publisher("encoder", Int32, queue_size=1)
    rate = rospy.Rate(100)
    
    msg = Int32()
    while not rospy.is_shutdown():
        msg.data = encoder.position()
        encoder_pub.publish(msg)
        rate.sleep()
