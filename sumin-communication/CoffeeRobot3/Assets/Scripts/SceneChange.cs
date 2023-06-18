using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneChange : MonoBehaviour
{
    [SerializeField]
    public string next_scene_name;
    void Start()
    {

    }

    void Update()
    {

    }

    public void sceneChange()
    {
        SceneManager.LoadScene(next_scene_name);
    }
}