using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.IO;
using System.Net;
using System.Net.Sockets;


public class Server : MonoBehaviour
{
    public static string HOST = "localhost";
    public static int PORT = 10000;
    public static void Main()
    {
        IPHostEntry ih = Dns.GetHostEntry(HOST);

        TcpListener tl = new TcpListener(ih.AddressList[0], PORT);
        tl.Start();

        Debug.Log("대기합니다");

        while(true)
        {
            TcpClient tc = tl.AcceptTcpClient();
            StreamWriter sw = new StreamWriter(tc.GetStream());
            Debug.Log("이쪽은 서버입니다.");

            sw.Flush();
            sw.Close();
            tc.Close();
            break;
        }
    }


    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
