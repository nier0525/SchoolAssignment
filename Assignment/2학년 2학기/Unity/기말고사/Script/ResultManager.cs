using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.SceneManagement;

public class ResultManager : MonoBehaviour
{
    public AudioSource Button_Sound;

    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 60;

        Cursor.lockState = CursorLockMode.None;
        Cursor.visible = true;
    }

    public void Return_Title()
    {
        Button_Sound.Play();
        SceneManager.LoadScene("Title");
    }

    public void Exit_Game()
    {
        Button_Sound.Play();
        Application.Quit();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
