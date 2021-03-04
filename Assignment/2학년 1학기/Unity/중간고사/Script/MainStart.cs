using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainStart : MonoBehaviour
{

    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 60;
        GameObject.Find("FadeIn").GetComponent<Fadein>().StartFadeAnim();
    }

    // Update is called once per frame
    void FixedUpdate()
    {

    }
}
