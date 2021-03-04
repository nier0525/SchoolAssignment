using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class icebolt : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Destroy(gameObject, 6);
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(-4f * Time.deltaTime, 0, 0);
    }
}
