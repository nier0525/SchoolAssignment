using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public class JumpPowerValue : MonoBehaviour
{
    Slider var;

    // Start is called before the first frame update
    void Start()
    {
        var = gameObject.GetComponent<Slider>();

        var.minValue = 3f;
        var.maxValue = 7f;

        var.value = PlayerPrefs.GetFloat("Jump", 5f);
    }

    // Update is called once per frame
    void Update()
    {
        PlayerPrefs.SetFloat("Jump", var.value);
    }
}
