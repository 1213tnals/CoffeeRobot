using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class send_stat : MonoBehaviour
{
    public GameObject LIGHT;
    int count = 0;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void statChange()
    {
        count++;
        if (count % 2 == 1)
        {
            LIGHT.GetComponent<blink_script>().blink_light(1);
        }
        else
        {
            LIGHT.GetComponent<blink_script>().blink_light(2);
        }
            
    }
}
