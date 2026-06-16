using UnityEngine;
using System.Net.Sockets;
using System.IO;
using System.Threading;
using System.Globalization;
using TMPro;

public class SerialMPU : MonoBehaviour
{
    public string esp32IP = "10.227.41.211";
    public int puerto = 5000;

    public float velocidad = 2f;
    public float alturaFija = 1f;
    public float zonaMuertaMovimiento = 3f;
    public float suavizado = 20f;

    public TextMeshProUGUI txtConexion;
    public TextMeshProUGUI txtPitch;
    public TextMeshProUGUI txtRoll;
    public TextMeshProUGUI txtIP;

    TcpClient client;
    StreamReader reader;
    Thread hiloLectura;

    float pitch = 0f;
    float roll = 0f;

    bool conectado = false;
    bool ejecutando = true;

    void Start()
    {
        try
        {
            client = new TcpClient(esp32IP, puerto);
            reader = new StreamReader(client.GetStream());

            conectado = true;
            ejecutando = true;

            hiloLectura = new Thread(LeerDatos);
            hiloLectura.Start();

            Debug.Log("Conectado al ESP32 por WiFi");
        }
        catch
        {
            conectado = false;
            Debug.LogError("No se pudo conectar al ESP32 por WiFi");
        }
    }

    void LeerDatos()
    {
        while (ejecutando && conectado)
        {
            try
            {
                string data = reader.ReadLine();

                if (data != null)
                {
                    string[] values = data.Split(',');

                    if (values.Length == 2)
                    {
                        pitch = float.Parse(values[0], CultureInfo.InvariantCulture);
                        roll = float.Parse(values[1], CultureInfo.InvariantCulture);
                    }
                }
            }
            catch
            {
                conectado = false;
            }
        }
    }

    void Update()
    {
        float pitchLocal = pitch;
        float rollLocal = roll;

        if (Mathf.Abs(pitchLocal) < 2f)
            pitchLocal = 0f;

        if (Mathf.Abs(rollLocal) < 2f)
            rollLocal = 0f;

        pitchLocal = Mathf.Clamp(pitchLocal, -45f, 45f);
        rollLocal = Mathf.Clamp(rollLocal, -45f, 45f);

        Quaternion targetRotation = Quaternion.Euler(
            pitchLocal,
            0f,
            -rollLocal
        );

        transform.rotation = Quaternion.Slerp(
            transform.rotation,
            targetRotation,
            Time.deltaTime * suavizado
        );

        Vector3 direccionMovimiento = Vector3.zero;

        if (pitchLocal > zonaMuertaMovimiento)
            direccionMovimiento += Vector3.forward;
        else if (pitchLocal < -zonaMuertaMovimiento)
            direccionMovimiento += Vector3.back;

        if (rollLocal > zonaMuertaMovimiento)
            direccionMovimiento += Vector3.right;
        else if (rollLocal < -zonaMuertaMovimiento)
            direccionMovimiento += Vector3.left;

        if (direccionMovimiento != Vector3.zero)
        {
            direccionMovimiento.Normalize();
            transform.position += direccionMovimiento * velocidad * Time.deltaTime;
        }

        Vector3 pos = transform.position;
        pos.y = alturaFija;
        transform.position = pos;

        if (txtConexion != null)
            txtConexion.text = conectado ? "Estado: Conectado" : "Estado: Desconectado";

        if (txtPitch != null)
            txtPitch.text = "Pitch: " + pitchLocal.ToString("F2") + "°";

        if (txtRoll != null)
            txtRoll.text = "Roll: " + rollLocal.ToString("F2") + "°";

        if (txtIP != null)
            txtIP.text = "IP: " + esp32IP;
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
