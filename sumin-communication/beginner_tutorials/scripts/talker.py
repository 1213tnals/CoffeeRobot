import rospy
import serial
import time
from std_msgs.msg import Int32

py_serial = serial.Serial(port='/dev/ttyACM1', baudrate=9600,)

def talker():
    pub = rospy.Publisher('jetson', Int32, queue_size=1)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    timer10 = 0

    while(1):
        if py_serial.readable():
            sending = py_serial.readline()
            sending_int = int(sending[:len(sending)-1].decode())
            pub.publish(sending_int)
            rospy.loginfo(sending_int)   

            if (sending_int==100):
                sending2_int = sending_int
                start_time = time.time()
                while (timer10 <= 10 and sending2_int == 100):# or sending2_int!=99):
                    sending2 = py_serial.readline()
                    sending2_int = int(sending[:len(sending)-1].decode())
                    end_time = time.time()
                    timer10 = end_time - start_time
                    rospy.loginfo(timer10)
                sending_int = sending2_int
            
                #time.sleep(3)
                #pub.publish(sending_int)
        #time.sleep(0.1)
        #rate.sleep()

if __name__ == '__main__':
    talker()
