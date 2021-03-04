using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Quest : MonoBehaviour
{
    public GameObject Izual, Hepasto, Shield;
    public AudioSource ClearSound;
    public Text Quest_Name;
    public Text Quest_Contents;    
    
    public static int Q;


    public static void Next_Quest()
    {
        Q++;
        if (Q == 3)
        {
            QuestAnimation.clear_Q1 = true;
        }
        if (Q == 6)
        {
            QuestAnimation.clear_Q2 = true;
        }
        if (Q == 8)
        {
            QuestAnimation.clear_Q3 = true;
        }
    }

    private void Start()
    {
        Q = 0;
    }

    void Update()
    {
        //if (Input.GetKeyDown(KeyCode.W))
        //{
        //    Next_Quest();
        //}

        switch (Q)
        {
            case 0:
                Quest_Name.text = "타락한 천사";
                Quest_Contents.text = "티리엘과 대화하시오.";
                break;
            case 1:
                Izual.SetActive(true);
                Quest_Contents.text = "이주얼의 영혼을 가지고 있는 악마를\n제거하십시오.";
                break;
            case 2:
                Quest_Contents.text = "이주얼의 영혼과 대화하시오."; // 1번퀘 비활
                break;
            case 3:
                Quest_Name.text = "불길 강의 대장장이";
                Quest_Contents.text = "해드리엘과 대화하시오.";
                break;
            case 4:
                Hepasto.SetActive(true);
                Quest_Contents.text = "디아블로의 봉인을 푸는 열쇠인\n대장장이 헤파스토를 처치하십시오.";
                break;
            case 5:
                Quest_Contents.text = "해드리엘과 대화하시오.";   // 2번퀘 비활
                break;
            case 6:
                Quest_Name.text = "공포의 결말";
                Quest_Contents.text = "지옥에서 부활한 공포의 군주\n디아블로를 처치하십시오!!";
                Shield.SetActive(false);
                break;
            case 7:
                Quest_Contents.text = "마을로 돌아가 티리엘과 대화하십시오."; // 3번퀘 비활
                break;
            case 8:
                Quest_Contents.text = "퀘스트 완료";
                break;
        }

        
    }
}
