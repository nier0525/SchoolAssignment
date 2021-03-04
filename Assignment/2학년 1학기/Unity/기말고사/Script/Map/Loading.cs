using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Loading : MonoBehaviour
{
    public Animator Wait;
    public Slider bar;

    bool isdone;
    float timer;
    AsyncOperation async;


    // Start is called before the first frame update
    void Start()
    {
        Wait.GetComponent<Animator>().SetBool("Wait", false);
        StartCoroutine(StartLoad());
        isdone = false;
        timer = 0f;
    }

    private void Awake()
    {
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = 60;
    }

    IEnumerator StartLoad()
    {
        yield return null;
        async = SceneManager.LoadSceneAsync("DiabloMap");
        async.allowSceneActivation = false;

        timer = 0f;

        while (!async.isDone)
        {
            yield return null;
            bar.value = async.progress;

            if (async.progress >= 0.9f)
            {
                Wait.GetComponent<Animator>().SetBool("Wait", true);
                timer += Time.deltaTime;

                if (timer >= 1f)
                {
                    async.allowSceneActivation = true;
                }
            }
        }
    }

    // Update is called once per frame
    void FixedUpdate()
    {

    }
}
