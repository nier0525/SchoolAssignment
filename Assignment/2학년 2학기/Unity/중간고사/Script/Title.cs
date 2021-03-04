using System.Collections;
using System.Collections.Generic;
using UnityEngine;



public class Title : MonoBehaviour
{
    public AudioSource BGM;
    public float audiosound = 1f;

    public GameObject[] Stage;
    public GameObject[] Stage_not;

    public GameObject elilogue, extra;

    // Start is called before the first frame update
    void Start()
    {
        //Screen.SetResolution(Screen.width, Screen.width * 16 / 9, false);

        GameObject.Find("Fade").GetComponent<Fadein>().StartFadeAnim();

        //PlayerPrefs.SetInt("Stage", 0);
        //PlayerPrefs.SetInt("Story", 0);

        for (int i = 0; i < Stage.Length; i++)
        {
            Stage[i].SetActive(false);
        }

        if (PlayerPrefs.GetInt("NowStage") == 4 || PlayerPrefs.GetInt("NowStage") == 5)
        {
            PlayerPrefs.SetInt("NowStage", 0);
        }
    }

    // Update is called once per frame
    void Update()
    {
        BGM.volume = audiosound;

        for (int i = 0; i <= PlayerPrefs.GetInt("Stage", 0); i++)
        {
            Stage[i].SetActive(true);
        }

        //for (int i = 0; i < 3; i++)
        //{
        //    Stage[i].SetActive(true);
        //}
        
        // ----------------------------------------------------------------------------

        for (int i = 0; i < 4;i++)
        {
            if(Stage[i+1].activeInHierarchy)
            {
                Stage_not[i].SetActive(false);
            }
        }

        if (PlayerPrefs.GetInt("Story") >= 2)
        {
            elilogue.SetActive(true);
        }

        if (PlayerPrefs.GetInt("Story") >= 4)
        {
            extra.SetActive(true);
        }

        if (Input.GetKeyDown(KeyCode.Escape))
        {
            GameObject.Find("Close").GetComponent<ButtonEvent>().QuitApplication();
        }
    }
}
