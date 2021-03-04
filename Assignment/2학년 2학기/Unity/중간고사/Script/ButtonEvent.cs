using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class ButtonEvent : MonoBehaviour
{
    public string scenename;
    public AudioSource Click;

    public GameObject Stage, Prologue, Epilogue, Extra, Main;

    bool attack_active;

    // Start is called before the first frame update
    void Start()
    {
        attack_active = true;
    }

    public void ReSetJumpPower()
    {
        GameObject.Find("JumpVar").GetComponent<Slider>().value = 5f;
    }

    public void PlayerAttack()
    {
        if (attack_active)
        {
            Debug.Log("Attack");
            GameObject.Find("Player").GetComponent<Player>().FireActive();
            attack_active = false;
        }
        else if (!attack_active)
        {
            Debug.Log("NoAttack");
            GameObject.Find("Player").GetComponent<Player>().FireUnActive();
            attack_active = true;
        }
    }

    public void PlayerSkill()
    {
        GameObject.Find("Player").GetComponent<Player>().Skillactive();
    }

    public void StageMenu()
    {
        if (PlayerPrefs.GetInt("Story") == 0)
        {
            Click.Play();

            GameObject.Find("BGM").GetComponent<AudioSource>().Stop();
            GameObject.Find("BGM_Pro").GetComponent<AudioSource>().Play();
            StoryManager.index = 0;

            if (Prologue.activeInHierarchy == false)
            {
                Prologue.SetActive(true);
                Main.SetActive(false);
            }
        }

        else
        {
            Stage.SetActive(true);
            Main.SetActive(false);
        }
    }

    public void ExtraScene()
    {
        Click.Play();

        if (PlayerPrefs.GetInt("Story") == 3)
        {
            StoryManager.storyreview_3 = true;

            Extra.SetActive(true);
            Stage.SetActive(false);

            GameObject.Find("BGM").GetComponent<AudioSource>().Stop();
            GameObject.Find("BGM_Extra").GetComponent<AudioSource>().Play();
        }

        else
        {
            GameObject.Find("Fade").GetComponent<FadeOut>().StartFadeAnim();
            StartCoroutine(FadeOut());
        }
    }

    public void PrologueReview()
    {
        Click.Play();

        StoryManager.storyreview_0 = true;

        Stage.SetActive(false);
        Prologue.SetActive(true);

        GameObject.Find("BGM").GetComponent<AudioSource>().Stop();
        GameObject.Find("BGM_Pro").GetComponent<AudioSource>().Play();
    }

    public void EpilogueReview()
    {
        Click.Play();

        StoryManager.storyreview_2 = true;

        Stage.SetActive(false);
        Epilogue.SetActive(true);

        GameObject.Find("BGM").GetComponent<AudioSource>().Stop();
        GameObject.Find("BGM_Epi").GetComponent<AudioSource>().Play();
    }

    public void ExtraReview()
    {
        Click.Play();

        StoryManager.storyreview_3 = true;

        Stage.SetActive(false);
        Extra.SetActive(true);

        GameObject.Find("BGM").GetComponent<AudioSource>().Stop();
        GameObject.Find("BGM_Extra").GetComponent<AudioSource>().Play();
    }

    public void Exit()
    {
        Click.Play();

        Application.Quit();
    }

    public void QuitApplication()
    {
        Click.Play();

        if (!Prologue.activeInHierarchy && !Epilogue.activeInHierarchy &&
            !Extra.activeInHierarchy && !Stage.activeInHierarchy)
        {
            Application.Quit();
        }

        else if (Prologue.activeInHierarchy)
        {
            Prologue.SetActive(false);
            Stage.SetActive(true);

            GameObject.Find("BGM").GetComponent<AudioSource>().Play();
            GameObject.Find("BGM_Pro").GetComponent<AudioSource>().Stop();

            if (PlayerPrefs.GetInt("Story") == 0)
            {
                PlayerPrefs.SetInt("Story", 1);
            }

            StoryManager.storyreview_0 = false;
            StoryManager.index = 0;
        }

        else if (Epilogue.activeInHierarchy)
        {
            Epilogue.SetActive(false);
            Stage.SetActive(true);

            GameObject.Find("BGM").GetComponent<AudioSource>().Play();
            GameObject.Find("BGM_Epi").GetComponent<AudioSource>().Stop();

            StoryManager.storyreview_2 = false;
            StoryManager.index = 0;
        }


        else if (Extra.activeInHierarchy)
        {
            Extra.SetActive(false);
            Stage.SetActive(true);

            GameObject.Find("BGM").GetComponent<AudioSource>().Play();
            GameObject.Find("BGM_Extra").GetComponent<AudioSource>().Stop();
            GameObject.Find("BGM_Extra2").GetComponent<AudioSource>().Stop();

            if (PlayerPrefs.GetInt("Story") == 3)
            {
                PlayerPrefs.SetInt("Story", 4);
            }

            StoryManager.storyreview_3 = false;
            StoryManager.index = 0;
        }

        else if (Stage.activeInHierarchy)
        {
            Stage.SetActive(false);
            Main.SetActive(true);
        }

        //UnityEditor.EditorApplication.isPlaying = false;
    }

    public void ChangeScene()
    {
        Click.Play();
        SceneManager.LoadScene(scenename);
        Time.timeScale = 1;
    }

    public void ExitOption()
    {
        Click.Play();

        GameObject.Find("GameManager").GetComponent<GameManager>().option = false;

        for (int i = 0; i < GameObject.Find("GameManager").GetComponent<GameManager>().OptionButton.Length; i++)
        {
            GameObject.Find("GameManager").GetComponent<GameManager>().OptionButton[i].SetActive(false);
        }

        Time.timeScale = 1;
    }

    public void StartGame()
    {
        Click.Play();
        StartCoroutine(FadeOut());
    }

    IEnumerator FadeOut()
    {
        while (true)
        {
            GameObject.Find("TitleInit").GetComponent<Title>().audiosound -= 0.1f;
            yield return new WaitForSeconds(0.3f);

            if (GameObject.Find("TitleInit").GetComponent<Title>().audiosound <= 0f)
            {
                SceneManager.LoadScene(scenename);
                break;
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
