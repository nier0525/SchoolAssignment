using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Lose : MonoBehaviour
{
    public AudioSource BGM;
    public float audiosound = 1f;

    // Start is called before the first frame update
    void Start()
    {
        //Screen.SetResolution(Screen.width, Screen.width * 16 / 9, false);

        GameObject.Find("Fade").GetComponent<Fadein>().StartFadeAnim();
    }

    // Update is called once per frame
    void Update()
    {
        BGM.volume = audiosound;

        if (Input.GetKeyDown(KeyCode.Escape))
        {
            Application.Quit(1);
        }

    }
}
