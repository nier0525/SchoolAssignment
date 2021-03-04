using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class well : MonoBehaviour
{
    float speed = 0.0f;
    float delta = 500.0f;
    bool on;

    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 60;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        delta += 1;

        RandomWell();

        if (on)
        {
            if (transform.position.y > 2.42f)
                speed = 0;
            else
                speed = 0.1f;

            transform.Translate(0, speed, 0);
        }

        if (!on)
        {
            if (transform.position.y < -2.52f)           
                speed = 0;           
            else
                speed = 0.1f;

            transform.Translate(0, -speed, 0);
        }
    }

    void RandomWell()
    {
        if (delta >= 500)
        {
            int temp = Random.Range(1, 6);
            if (temp == 1 || temp == 2)
                on = true;
            else
                on = false;
            delta = 0.0f;
        }
    }
}
