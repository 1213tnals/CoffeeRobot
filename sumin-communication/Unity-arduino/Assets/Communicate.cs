using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class Communicate : MonoBehaviour
{
    public enum PorNumber
    {
        COM1, COM2, COM3, COM4,
        COM5, COM6, COM7, COM8,
        COM9, COM10, COM11, COM12,
        COM13, COM14, COM15, COM16,
    }

    private SerialPort arduino;

    [SerializeField]
    private PorNumber portNumber = PorNumber.COM1;
    [SerializeField]
    private string baudRate = "9600";

    private bool LedRState = false;
    private bool LedYState = false;
    private bool LedGState = false;


    void Start()
    {
        arduino = new SerialPort(portNumber.ToString(), int.Parse(baudRate));          //SerialPort 동적할당
    }

    // Update is called once per frame
    void Update()
    {
    }

    public void LedToggle(string data)
    {
        if (!arduino.IsOpen)
            arduino.Open();                                  //아두이노와의 통신 연결

        bool LedState = false;

        switch(data)
        {
            case "R":
                LedState = LedRState;
                break;
            case "Y":
                LedState = LedYState;
                break;
            case "G":
                LedState = LedGState;
                break;
        }

        if (LedState == false)
        {
            arduino.Write(data);
            LedState = !LedState;
        }
        else if(LedState)
        {
            arduino.Write(data.ToLower());
            LedState = !LedState;
        }

        switch (data)
        {
            case "R":
                LedRState = LedState;
                break;
            case "Y":
                LedYState = LedState;
                break;
            case "G":
                LedGState = LedState;
                break;
        }
    }

}
