import rospy
import serial
import time
from std_msgs.msg import Int32

py_serial = serial.Serial(port='/dev/ttyUSB0', baudrate=9600,)

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

            # if (sending_int==100):
            #     sending2_int = sending_int
            #     rospy.loginfo(sending2_int)
            #     start_time = time.time()

                # while (timer10 <= 10 and sending_int == 100):# or sending2_int!=99):
                #     rospy.loginfo("in while2")
                    
                #     if py_serial2.readable():
                #         sending = py_serial2.readligitne()
                #         sending_int = int(sending[:len(sending)-1].decode())
                        
                #         rospy.loginfo(sending_int)
                #         end_time = time.time()
                #         rospy.loginfo(end_time)
                #         timer10 = end_time - start_time
                #         rospy.loginfo(timer10)

                #sending_int = sending2_int
                # rospy.loginfo("escape!")

                # if(timer10 > 10):
                #     #시리얼로 아두이노에 cmd='Z' 먹이기
                #     command = "Z"
                #     py_serial.write(command.encode())
                #     time.sleep(0.1)
                    
                #time.sleep(3)
                #pub.publish(sending_int)
        #time.sleep(0.1)
        #rate.sleep()

if __name__ == '__main__':
    talker()
