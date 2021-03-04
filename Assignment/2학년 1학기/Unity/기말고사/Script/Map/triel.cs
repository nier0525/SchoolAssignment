using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class triel : MonoBehaviour
{
    public GameObject name, balloon, Menu, Menu2 , Talk, TextRect;
    public Text TalkText, NameText;
    public AudioSource[] Voice;

    SpriteRenderer tr_spr;
    Color c , c_temp;

    public static bool Enter = false;
    public static bool Click = false;
    static public bool Stay = false;

    public static bool SoundCheak = false;
    public static bool Last = false;

    int index = 0;

    private void Start()
    {
        tr_spr = gameObject.GetComponent<SpriteRenderer>();
        c_temp = tr_spr.color;
        name.SetActive(false);
        balloon.SetActive(false);
    }

    private void OnMouseDown()
    {
        Click = true;

        //if (!balloon.active && Enter)
        //{
        //    Debug.Log("ok");
        //}
    }


    //private void OnMouseUp()
    //{
    //    Click = false;
    //}

    private void OnMouseOver()
    {
        name.SetActive(true);
        GameObject.Find("NPC").GetComponent<NPC>().MouseIn();
        c = new Color(1, 1, 1, 1);
        tr_spr.color = c;
    }

    private void OnMouseExit()
    {
        name.SetActive(false);
        GameObject.Find("NPC").GetComponent<NPC>().MouseOut();
        c = c_temp;
        tr_spr.color = c;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
            Enter = true;
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            Stay = true;

            if (!Talk.active && Click)
            {
                GameObject.Find("Sorceress").GetComponent<PlayerControl>().Stop();
                NameText.text = "티리엘";
                Menu.SetActive(true);
                Click = false;
            }
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            Menu.SetActive(false);
            Menu2.SetActive(false);
            Enter = false;
            Stay = false;
        }
    }

    private void Update()
    {
        if (Click)
        {
            Vector2 TextPos = new Vector2(this.transform.position.x, this.transform.position.y + 1.2f);
            TextRect.transform.position = Camera.main.WorldToScreenPoint(TextPos);
        }


        if (NPCTalk.Intro && SoundCheak)
        {
            Voice[0].Play();
            TalkText.text = "그대를 다시 만나게 되어 반갑소, 영웅이여. 메피스토의 패배는 빛의 위대한 승리요!" +
                " 나는 그대가 결국 이곳까지 오리라고 믿었소. 지옥의 성채는 불지옥의 문 앞에 위치한 천계 군세의 마지막 보루요." +
                " 이곳은 수 천의 빛의 전사들의 피로 신성해졌소. 그리고 그들 중 많은 이들은 그대처럼 인간이었소. " +
                "이제 대악마들과의 최후의 결전이 다가오고 있소. 그리고 그대는 그들과 홀로 맞서야만 하오. " +
                "나는 그대에게 몇 마디 조언을 해주는 것 이상의 도움은 줄 수 없다오. 인간이여, 바로 그대의 승리의 순간이오." +
                " 빛이 그대를 지켜주고 천계의 신들이 그대가 갈 길을 비쳐주기를….";
            SoundCheak = false;
        }

        if (NPCTalk.Gossip && SoundCheak)
        {
            if(Quest.Q <= 2)
            {
                int index = Random.Range(1, 3);
                Debug.Log(index);

                Voice[index].Play();

                switch(index)
                {
                    case 1:
                        TalkText.text = "이주얼은 천사의 룬블레이드 애이저래스를 더 이상 갖고 있지 않지만, 여전히 엄청난 괴력과 마력을 갖고 있소." +
                            "그의 현재 상태로는 적과 친구를 구별할 수 없을 것이오.만약 그가 그대를 발견하면 틀림없이 적대적인 태도를 취할 것이오." +
                            "최대한 조심스럽게 진행하도록 하시오.";
                        break;

                    case 2:
                        TalkText.text = "머뭇거려서는 안 되오, 영웅이여. " +
                            "이주얼에게는 안식이 주어져야 하지만, 디아블로는 아직도 이 영역을 활보하고 있소. 어서 가시오. 서두르시오!";
                        break;
                }
            }

            else if (Quest.Q > 2 && Quest.Q <= 7)
            {
                Voice[3].Play();
                TalkText.text = "이주얼의 고통 받는 영혼에게 안식을 주어 고맙소, 영웅이여. " +
                    "빛이 그대를 지켜주고 천계의 신들이 그대가 갈 길을 비추어 주기를…. " +
                    "하지만 그대가 내게 하는 말이 사실이라면 우리가 시종일관 어리석은 짓만 한 게 아닌가 우려되오. " +
                    "이주얼은 나를 속이고 디아블로와 그의 형제가 소울 스톤을 사용하도록 도왔소. 이제 그들은 그 사용법을 완전히 익혔을 것이오." +
                    " 소울 스톤들이 합쳐진 힘을 지배하게 된 대악마들은 인간 세계를 지옥의 영원한 식민지로 만들 수 있소!";
            }

            else
            {
                Last = true;
            }

            SoundCheak = false;
        }

        if (!NPCTalk.Intro && !NPCTalk.Gossip)
        {
            for (int i = 0; i < Voice.Length; i++)
                Voice[i].Stop();
        }

        if (Quest.Q == 0 || Quest.Q == 7)
        {
            balloon.SetActive(true);
        }
        else
        {
            balloon.SetActive(false);
        }
    }
}
