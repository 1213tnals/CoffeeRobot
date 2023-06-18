using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using Int32Msg = RosMessageTypes.Std.Int32Msg;
using Unity.Robotics.ROSTCPConnector;

public class MakingSub : MonoBehaviour
{
    [SerializeField]
    public string next_scene_name = "Finish";
    public string topicName = "jetson";
    int jetson_sending;

    ROSConnection m_Ros;


    void Start()
    {
        m_Ros = ROSConnection.GetOrCreateInstance();

        m_Ros.Subscribe<Int32Msg>(topicName, JetsonCallback);
    }

    // Update is called once per frame
    void Update()
    {
        if (jetson_sending==5)
            SceneManager.LoadScene(next_scene_name);
    }


    private void JetsonCallback(Int32Msg data)
    {
        jetson_sending = data.data;
    }
}
