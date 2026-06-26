using UnityEngine;
using TMPro;

public class UIManager : MonoBehaviour
{
    public NetworkManager network;

    public TextMeshProUGUI txtConexion;
    public TextMeshProUGUI txtPitch;
    public TextMeshProUGUI txtRoll;
    public TextMeshProUGUI txtIP;

    void Update()
    {
        if (network == null)
            return;

        if (txtConexion != null)
            txtConexion.text = network.Conectado ? "Estado: Conectado" : "Estado: Desconectado";

        if (txtPitch != null)
            txtPitch.text = "Pitch: " + network.Pitch.ToString("F2") + "°";

        if (txtRoll != null)
            txtRoll.text = "Roll: " + network.Roll.ToString("F2") + "°";

        if (txtIP != null)
            txtIP.text = "IP: " + network.esp32IP;
    }
}