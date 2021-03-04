using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Start : MonoBehaviour
{
    float animTime = 4f;
    float time = 0f;

    bool timecheck = false;

    public void Starttime()
    {
        GameObject.Find("Select").GetComponent<AudioSource>().Play();
        if (timecheck == true)
            return;
        StartCoroutine("Onclick");
    }

    IEnumerator Onclick()
    {
        timecheck = true;
        time = 0f;

        while (time < 1f)
        {
            time += Time.deltaTime / animTime;
            yield return null;
        }

        if (time > 1f)
        {
            SceneManager.LoadScene("Main");
        }

        timecheck = false;
    }

    public void OnclickExit()
    {
        GameObject.Find("Select").GetComponent<AudioSource>().Play();
      //  UnityEditor.EditorApplication.isPlaying = false;
        Application.Quit();
    }

    // Update is called once per frame
    void Update()
    {

    }
}
