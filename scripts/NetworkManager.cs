using UnityEngine;
using System;
using System.Net.Sockets;
using System.IO;
using System.Threading;
using System.Globalization;

public class NetworkManager : MonoBehaviour
{
    public string esp32IP = "10.227.41.120";
    public int puerto = 5000;

    public float Pitch { get; private set; }
    public float Roll { get; private set; }
    public bool Conectado { get; private set; }

    private TcpClient client;
    private StreamReader reader;
    private Thread hiloLectura;
    private bool ejecutando = true;

    void Start()
    {
        Conectar();
    }

    void Conectar()
    {
        try
        {
            client = new TcpClient(esp32IP, puerto);
            reader = new StreamReader(client.GetStream());

            Conectado = true;
            ejecutando = true;

            hiloLectura = new Thread(LeerDatos);
            hiloLectura.Start();

            Debug.Log("Conectado al ESP32 por WiFi");
        }
        catch
        {
            Conectado = false;
            Debug.LogError("No se pudo conectar al ESP32 por WiFi");
        }
    }

    void LeerDatos()
    {
        while (ejecutando && Conectado)
        {
            try
            {
                string data = reader.ReadLine();

                if (data != null)
                {
                    string[] values = data.Split(',');

                    if (values.Length == 2)
                    {
                        Pitch = float.Parse(values[0], CultureInfo.InvariantCulture);
                        Roll = float.Parse(values[1], CultureInfo.InvariantCulture);
                    }
                }
            }
            catch
            {
                Conectado = false;
            }
        }
    }

    void OnApplicationQuit()
    {
        ejecutando = false;

        if (hiloLectura != null && hiloLectura.IsAlive)
            hiloLectura.Abort();

        if (reader != null)
            reader.Close();

        if (client != null)
            client.Close();
    }
}
