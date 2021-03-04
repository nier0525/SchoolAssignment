using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class QuestAnimation : MonoBehaviour
{
    public GameObject QuestObj;
    public GameObject Q1, Q2, Q3;
    public Sprite Sel_Q1, Sel_Q2, Sel_Q3;

    public Sprite[] Q1_Ani, Q2_Ani, Q3_Ani;

    public static bool clear_Q1, clear_Q2, clear_Q3;

    // Start is called before the first frame update
    void Start()
    {
        clear_Q1 = false;
        clear_Q2 = false;
        clear_Q3 = false;
    }

    private void Awake()
    {
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = 60;
    }

    IEnumerator Q1_Animation()
    {
        int temp = 0;

        while (temp < Q1_Ani.Length)
        {
            yield return new WaitForSeconds(0.05f);
            Q1.GetComponent<Image>().sprite = Q1_Ani[temp++];
        }
    }

    IEnumerator Q2_Animation()
    {
        int temp = 0;

        while (temp < Q2_Ani.Length)
        {
            yield return new WaitForSeconds(0.05f);
            Q2.GetComponent<Image>().sprite = Q2_Ani[temp++];
        }
    }

    IEnumerator Q3_Animation()
    {
        int temp = 0;

        while (temp < Q3_Ani.Length)
        {
            yield return new WaitForSeconds(0.05f);
            Q3.GetComponent<Image>().sprite = Q3_Ani[temp++];
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (Quest.Q <= 2)
        {
            Q1.GetComponent<Image>().sprite = Sel_Q1;
        }

        else if (Quest.Q >2 && Quest.Q <= 5)
        {
            Q2.GetComponent<Image>().sprite = Sel_Q2;
        }

        else if (Quest.Q >5 && Quest.Q <= 7)
        {
            Q3.GetComponent<Image>().sprite = Sel_Q3;
        }

        if (QuestObj.active)
        {
            if (clear_Q1)
            {
                StartCoroutine(Q1_Animation());
                clear_Q1 = false;
            }

            if (clear_Q2)
            {
                StartCoroutine(Q2_Animation());
                clear_Q2 = false;
            }

            if (clear_Q3)
            {
                StartCoroutine(Q3_Animation());
                clear_Q3 = false;
            }
        } 
    }
}
