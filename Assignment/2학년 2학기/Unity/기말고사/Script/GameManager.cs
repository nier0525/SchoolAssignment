using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public GameObject UI;
    public GameObject[] Tower;
    public AudioSource Button_Sound;
    Text HUD_Score;

    static public float Score;
    int Tower_Destory;

    static public  bool menu;

    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 60;

        Score = 0;
        Tower_Destory = 0;

        menu = false;

        HUD_Score = GameObject.Find("Score_HUD").GetComponent<Text>();

        if (HUD_Score != null)
            HUD_Score.text = "Score : " + Score;

        UI.SetActive(false);
    }

    public void UpdateScore(float _Score)
    {
        Score += _Score;
        HUD_Score.text = "Score : " + (int)Score;
    }

    public void TowerCheak(GameObject arg)
    {
        ++Tower_Destory;

        if (Tower_Destory == Tower.Length)
            SceneManager.LoadScene("Lose");
    }

    public void Continue()
    {
        Button_Sound.Play();

        Cursor.lockState = CursorLockMode.Locked;
        Cursor.visible = false;
        Time.timeScale = 1;

        UI.SetActive(false);
        menu = false;
    }

    public void QuitGame()
    {
        Button_Sound.Play();

        Cursor.lockState = CursorLockMode.None;
        Cursor.visible = true;
        Time.timeScale = 1;

        SceneManager.LoadScene("Title");
    }

    // Update is called once per frame
    void Update()
    {
        if (Score > 2500)
        {
            SceneManager.LoadScene("Win");
        }

        if (Input.GetKeyDown(KeyCode.Escape))
        {
            Button_Sound.Play();

            if (!menu)
            {
                GunScript.fadeout_value = 0f;

                Cursor.lockState = CursorLockMode.None;
                Cursor.visible = true;
                Time.timeScale = 0;

                UI.SetActive(true);
                menu = true;
            }

            else
            {
                Cursor.lockState = CursorLockMode.Locked;
                Cursor.visible = false;
                Time.timeScale = 1;

                UI.SetActive(false);
                menu = false;
            }
        }
        
    }
}
