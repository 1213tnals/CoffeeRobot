import rospy
import serial
import time
from std_msgs.msg import Int32

py_serial = serial.Serial(port='/dev/ttyUSB0', baudrate=9600,)

def talker():
    pub = rospy.Publisher('jetson', Int32, queue_size=1)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while(1):
        if py_serial.readable():
            sending = py_serial.readline()
            sending_int = int(sending[:len(sending)-1].decode())
            pub.publish(sending_int)
            rospy.loginfo(sending_int)   

            if (sending_int==0):
                time.sleep(3)
                pub.publish(sending_int)
        #time.sleep(0.1)
        #rate.sleep()

if __name__ == '__main__':
    talker()
