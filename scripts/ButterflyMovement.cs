using UnityEngine;

public class ButterflyMovement : MonoBehaviour
{
    public NetworkManager network;

    public float velocidad = 2f;
    public float alturaFija = 1f;
    public float zonaMuerta = 3f;
    public float suavizado = 20f;

    void Update()
    {
        if (network == null)
            return;

        float pitch = network.Pitch;
        float roll = network.Roll;

        if (Mathf.Abs(pitch) < 2f)
            pitch = 0;

        if (Mathf.Abs(roll) < 2f)
            roll = 0;

        pitch = Mathf.Clamp(pitch, -45f, 45f);
        roll = Mathf.Clamp(roll, -45f, 45f);

        Quaternion targetRotation =
            Quaternion.Euler(
                pitch,
                0f,
                -roll
            );

        transform.rotation = Quaternion.Slerp(
            transform.rotation,
            targetRotation,
            Time.deltaTime * suavizado
        );

        Vector3 direccion = Vector3.zero;

        if (pitch > zonaMuerta)
            direccion += Vector3.forward;

        else if (pitch < -zonaMuerta)
            direccion += Vector3.back;

        if (roll > zonaMuerta)
            direccion += Vector3.left;

        else if (roll < -zonaMuerta)
            direccion += Vector3.right;

        if (direccion != Vector3.zero)
        {
            direccion.Normalize();

            transform.position +=
                direccion *
                velocidad *
                Time.deltaTime;
        }

        Vector3 pos = transform.position;

        pos.y = alturaFija;

        transform.position = pos;
    }
}
