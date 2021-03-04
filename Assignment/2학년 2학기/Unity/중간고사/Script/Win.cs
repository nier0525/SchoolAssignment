using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public class Win : MonoBehaviour
{
    public GameObject[] Star;
    public GameObject Fade;
    public AudioSource BGM;
    public Text score;

    public float audiosound = 1f;

    public int stage_clear;
    public int stage_star;
    public int stage_high;

    // Start is called before the first frame update
    void Start()
    {
        //Screen.SetResolution(Screen.width, Screen.width * 16 / 9, false);

        stage_high = PlayerPrefs.GetInt("Stage1_Score", 0);

        if (stage_high < GameManager.score)
        {
            PlayerPrefs.SetInt("Stage1_Score", GameManager.score);
            stage_high = GameManager.score;
        }

        Fade.GetComponent<Fadein>().StartFadeAnim();

        stage_clear = 1;

        switch (PlayerPrefs.GetInt("NowStage"))
        {

            case 1:
                if (GameManager.score >= 18000)
                {
                    stage_clear++;
                }
                break;

            case 2:
                if (GameManager.score >= 27500)
                {
                    stage_clear++;
                }
                break;

            case 3:
                if (GameManager.score >= 40000)
                {
                    stage_clear++;
                }
                break;

            case 4:
                if (GameManager.score >= 52000)
                {
                    stage_clear++;
                }
                break;

            case 5:
                if (GameManager.score >= 90000)
                {
                    stage_clear++;
                }
                break;
        }

        if (Player.life == 3)
        {
            stage_clear++;
        }


        if (PlayerPrefs.GetInt("NowStage") == 1)
        {
            if (PlayerPrefs.GetInt("Stage1_Star", 0) < stage_clear)
            {
                PlayerPrefs.SetInt("Stage1_Star", stage_clear);
            }
        }

        if (PlayerPrefs.GetInt("NowStage") == 2)
        {
            if (PlayerPrefs.GetInt("Stage2_Star", 0) < stage_clear)
            {
                PlayerPrefs.SetInt("Stage2_Star", stage_clear);
            }
        }

        if (PlayerPrefs.GetInt("NowStage") == 3)
        {
            if (PlayerPrefs.GetInt("Stage3_Star", 0) < stage_clear)
            {
                PlayerPrefs.SetInt("Stage3_Star", stage_clear);
            }
        }

        stage_star = PlayerPrefs.GetInt("Stage1_Star") + PlayerPrefs.GetInt("Stage2_Star") + PlayerPrefs.GetInt("Stage3_Star");

        if (stage_star >= 7)
        {
            if (PlayerPrefs.GetInt("Stage") == 2)
            {
                PlayerPrefs.SetInt("Stage", 3);
            }
        }

        //Debug.Log(PlayerPrefs.GetInt("Stage1_Star"));
        //Debug.Log(PlayerPrefs.GetInt("Stage1_Score"));
    }

    // Update is called once per frame
    void Update()
    {
        BGM.volume = audiosound;
        score.text = "Score : " + GameManager.score;

        for (int i = 0; i < stage_clear; i++)
        {
            Star[i].SetActive(true);
        }

        if (Input.GetKeyDown(KeyCode.Escape))
        {
            Application.Quit(1);
        }
    }
}
