using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Robotics.ROSTCPConnector;
using Int32Msg = RosMessageTypes.Std.Int32Msg;    //+

public class HomePub : MonoBehaviour
{
    public string topicName = "unity/home";

    ROSConnection m_Ros;

    private Int32Msg user_recog;    //+

    void Start()
    {
        m_Ros = ROSConnection.GetOrCreateInstance();
        m_Ros.RegisterPublisher<Int32Msg>(topicName);    //+

        user_recog = new();
    }
    void Update()
    {
        
    }

    public void Click()
    {
        user_recog.data = 10;    //+

        m_Ros.Publish(topicName, user_recog);    //+
    }
}
