using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using Unity.Robotics.ROSTCPConnector;
using Int32Msg = RosMessageTypes.Std.Int32Msg;    //+

public class ShotSelect : MonoBehaviour
{
    [SerializeField]
    public string next_scene_name = "Making";
    public string topicName = "unity/order";

    ROSConnection m_Ros;

    private Int32Msg shot_num;    //+

    void Start()
    {
        m_Ros = ROSConnection.GetOrCreateInstance();
        m_Ros.RegisterPublisher<Int32Msg>(topicName);    //+

        shot_num = new();
    }

    void Update()
    {

    }

    public void shootHalf() 
    {
        shot_num.data = 1;    //+

        m_Ros.Publish(topicName, shot_num);    //+

        SceneManager.LoadScene(next_scene_name);
    }

    public void shootOne()
    {
        shot_num.data = 2;    //+

        m_Ros.Publish(topicName, shot_num);    //+

        SceneManager.LoadScene(next_scene_name);
    }

    public void shootTwo()
    {
        shot_num.data = 3;    //+

        m_Ros.Publish(topicName, shot_num);    //+

        SceneManager.LoadScene(next_scene_name);
    }
}