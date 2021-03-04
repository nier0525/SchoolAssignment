using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.SceneManagement;

public class TitleManager : MonoBehaviour
{
    public GameObject Explanation;
    public AudioSource Button_Sound;

    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 60;

        Explanation.SetActive(false);
    }

    public void GameStart()
    {
        Button_Sound.Play();
        SceneManager.LoadScene("Main");
    }

    public void GameExplanation()
    {
        Button_Sound.Play();
        Explanation.SetActive(true);
    }

    public void GameExplanation_Quit()
    {
        Button_Sound.Play();
        Explanation.SetActive(false);
    }

    public void GameExit()
    {
        Button_Sound.Play();
        Application.Quit();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
            if (Explanation.activeInHierarchy == true)
            {
                GameExplanation_Quit();
            }
    }
}
