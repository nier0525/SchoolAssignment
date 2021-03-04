using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MoveCamera : MonoBehaviour
{

    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 60;
    }

    // Update is called once per frame
    void Update()
    {
        if (transform.position.z > -18.16f)
        {
            transform.position = new Vector3(0.18f, 1.1f, -61.05f);
        }
        else
            transform.Translate(0, 0, 0.05f);

        if (transform.position.z > -29.50f)
        {
            GameObject.Find("FadeOut").GetComponent<FadeOut>().StartFadeAnim();
            GameObject.Find("Font").GetComponent<Fadein>().StartFadeAnim();
        }

        if (transform.position.z < -54)
        {
            GameObject.Find("FadeOut").GetComponent<Fadein>().StartFadeAnim();
            GameObject.Find("Font").GetComponent<FadeOut>().StartFadeAnim();
        }
        //if (transform.position.z < -62f)
        //{
        //    GameObject.Find("FadeIn").GetComponent<Fadein>().StartFadeAnim();
        //}

    }
}
