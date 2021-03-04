using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveObject : MonoBehaviour
{
    public GameObject []Object;
    public float MoveSpeed;

    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 30;
    }

    // Update is called once per frame
    void Update()
    {
        Object[0].transform.Translate(new Vector3(-(MoveSpeed * 2) * Time.deltaTime, 0, 0));
        Object[1].transform.Translate(new Vector3(-(MoveSpeed * 2) * Time.deltaTime, 0, 0));
        Object[2].transform.Translate(new Vector3(-MoveSpeed * Time.deltaTime, 0, 0));

        for (int i = 0; i < Object.Length; i++)
        {
            if (Object[i].transform.position.x < -20f)
            {
                Object[i].transform.position = new Vector2(18, Object[i].transform.position.y);
            } 
        }
    }
}
