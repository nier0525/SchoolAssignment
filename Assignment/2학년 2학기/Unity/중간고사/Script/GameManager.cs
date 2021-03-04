using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    public Text Score;
    public GameObject [] Hp;
    public GameObject[] OptionButton;
    public GameObject Boss, Deco, BossWarning;
    public Slider Mp;

    public AudioSource Wait, BGM, BOSSBGM2;

    static public int score;

    public bool option = false;
    static public bool boss = false;

    float bosslate;

    // Start is called before the first frame update
    void Start()
    {
        //Screen.SetResolution(Screen.width, Screen.width * 16 / 9, false);

        score = 0;

        for (int i = 0; i < Hp.Length; i++)
        {
            Hp[i].SetActive(true);
        }

        Mp.value = Player.Mana;

        if (PlayerPrefs.GetInt("NowStage") == 4)
        {
            Debug.Log("Hidden");
            bosslate = 2f;
        }

        else
        {
            bosslate = 45f;
        }

        StartCoroutine(BossLate());
        Application.targetFrameRate = 30;
    }

    IEnumerator BossLate()
    {
        yield return new WaitForSeconds(bosslate);
        BGM.Stop();
        Wait.Play();

        BossWarning.SetActive(true);
        yield return new WaitForSeconds(0.5f);
        BossWarning.SetActive(false);
        yield return new WaitForSeconds(0.5f);
        BossWarning.SetActive(true);
        yield return new WaitForSeconds(0.5f);
        BossWarning.SetActive(false);
        yield return new WaitForSeconds(0.5f);

        Boss.SetActive(true);
        Deco.SetActive(true);
        boss = true;
        BOSSBGM2.Play();

        if (PlayerPrefs.GetInt("NowStage") != 4)
        {
            GameObject.Find("GameManager").GetComponent<Spawner>().interval = 4f;
        }
    }

    public void OptionActive()
    {
        if (!option)
        {
            for (int i = 0; i < OptionButton.Length; i++)
            {
                OptionButton[i].SetActive(true);
            }

            Time.timeScale = 0;
            option = true;
        }

        else
        {
            for (int i = 0; i < OptionButton.Length; i++)
            {
                OptionButton[i].SetActive(false);
            }

            Time.timeScale = 1;
            option = false;
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            OptionActive();
        }

        Score.text = "Score : " + score;
        Mp.value = Player.Mana;

        switch(Player.life)
        {
            case 2:
                Hp[0].SetActive(false);
                break;

            case 1:
                Hp[1].SetActive(false);
                break;

            case 0:
                Hp[2].SetActive(false);
                break;
        }
    }
}
