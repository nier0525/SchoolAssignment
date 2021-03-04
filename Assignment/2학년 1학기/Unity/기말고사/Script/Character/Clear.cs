using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Clear : MonoBehaviour
{
    float time = 0f;
    bool timecheak = false;

    private void Start()
    {
        Application.targetFrameRate = 60;
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.name == "unitychan")
        {
            GameObject.Find("Clear").GetComponent<AudioSource>().Play();
            GameObject.Find("FadeIn").GetComponent<FadeOut>().StartFadeAnim();
            timecheak = true;   
        }
    }

    private void Update()
    {
        if (timecheak)
        {
            time += Time.deltaTime / 4f;
        }
        if (time > 1f)
        {
            SceneManager.LoadScene("Clear");
        }
    }
}
