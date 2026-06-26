using UnityEngine;

public class CameraFollow : MonoBehaviour
{
    public Transform objetivo;

    public Vector3 offset = new Vector3(0f, 4f, -10f);
    public float suavizado = 5f;

    void LateUpdate()
    {
        if (objetivo == null)
            return;

        Vector3 posicionDeseada = objetivo.position + offset;

        transform.position = Vector3.Lerp(
            transform.position,
            posicionDeseada,
            Time.deltaTime * suavizado
        );

        transform.LookAt(objetivo);
    }
}