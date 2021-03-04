using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class finish : MonoBehaviour
{
    float temp = 0;
    bool state = false;

    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 60;
        state = false;
        temp = 0;
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0) || Input.GetKeyDown(KeyCode.Escape))
        {
            state = true;
        }

        if (state)
        {
            temp += Time.deltaTime / 4f;
            if (temp > 1f)
            {
                GameObject.Find("FadeIn").GetComponent<FadeOut>().StartFadeAnim();
            }
            if (temp > 2f)
            {
                SceneManager.LoadScene("Start");
            }
        }
    }
}
