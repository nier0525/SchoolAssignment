using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Camera : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 60;
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            transform.Translate(0, 0, 4);
            transform.Rotate(0, 180, 0);
        }
        if (Input.GetMouseButtonUp(0))
        {
            transform.Translate(0, 0, 4);
            transform.Rotate(0, -180, 0);
        }
    }
}
