using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class blink_script : MonoBehaviour
{
    public Light light_;
    SerialPort portNo = new SerialPort("COM5", 9600);
    void Start()
    {
        portNo.Open();
        portNo.ReadTimeout = 5000;
    }

    // Update is called once per frame
    void Update()
    {
        if(portNo.IsOpen)
        {
            try
            {
                blink_light(portNo.ReadByte());
            }
            catch (System.Exception)
            {
                throw;
            }
        }
    }

    public void blink_light(int stat)
    {
        if(stat==1)
        {
            light_.enabled = true;
            print(1);
        }
        else if(stat==2)
        {
            light_.enabled = false;
            print(2);
        }
    }
}
