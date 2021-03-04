using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class cain : MonoBehaviour
{
    public GameObject name, Menu, Menu2 , TextRect;
    public Text TalkText, NameText;
    public AudioSource[] Voice;

    SpriteRenderer cain_spr;
    Color c, c_temp;

    static public bool Enter = false;
    static public bool Click = false;
    static public bool Stay = false;

    static public bool SoundCheak = false;

    private void Start()
    {
        cain_spr = gameObject.GetComponent<SpriteRenderer>();
        c_temp = cain_spr.color;
        name.SetActive(false);
    }

    private void OnMouseOver()
    {
        name.SetActive(true);
        GameObject.Find("NPC").GetComponent<NPC>().MouseIn();
        c = new Color(1, 1, 1, 1);
        cain_spr.color = c;
    }

    private void OnMouseDown()
    {
        Click = true;
    }

    private void OnMouseExit()
    {
        name.SetActive(false);
        GameObject.Find("NPC").GetComponent<NPC>().MouseOut();
        c = c_temp;
        cain_spr.color = c;
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
            if (Click)
            {
                GameObject.Find("Sorceress").GetComponent<PlayerControl>().Stop();
                NameText.text = "케인";
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
            TalkText.text = "자네는 이곳이 믿어지는가? 자네는 언젠가 지옥과 천국의 교차로에 서게 되리라고 감히 상상이나 해보았나? " +
                "이 지옥의 성채는 가히 기적일세. 하지만 자네의 여행은 아직 끝나지 않았네. " +
                "디아블로는 아직도 지옥을 활보하며 자신의 군대를 정비하고 있네. 그를 처치해야만 우리의 세계가 진정한 평화를 찾을 수 있을 것이네. " +
                "서두르게, 시간은 그 누구도 기다려주지 않네!";
            SoundCheak = false;
        }

        if (NPCTalk.Gossip && SoundCheak)
        {
            if (Quest.Q == 0)
            {
                Voice[1].Play();
                TalkText.text = "나는 괴짜 대천사 티리엘에 대해서 많이 읽었네. " +
                    "호라드림의 전승에서는 그의 인간에 대한 깊은 동정심과 꺼지지 않는 영혼의 불길 때문에 숭배를 받고 있네. " +
                    "소문에 의하면 그는 천계의 뜻을 거역하고 디아블로와 그의 형제들을 봉인할 수 있도록 소울 스톤들을 호라드림에게 주었다고 하네.";
            }

            if (Quest.Q > 0 && Quest.Q <=2)
            {
                int index = Random.Range(2, 4);
                Voice[index].Play();

                switch(index)
                {
                    case 2:
                        TalkText.text = "티리엘이 자네에게 타락한 이주얼과 싸워달라고 부탁했나? 그는 자네의 능력을 높이 사고 있는 모양이군! " +
                            "난 자네가 자신이 뭘 하고 있는지 알고 있으리라 생각하지만, 조심하게. 자넨 우리의 마지막 희망일세.";
                        break;

                    case 3:
                        TalkText.text = "타락한 천사를 찾는 데 애먹고 있나? 서두르게. 거대한 악이 이 주위의 공기에 배어들기 시작한 것 같네.";
                        break;
                }
            }

            if (Quest.Q > 2 && Quest.Q <= 5)
            {
                Voice[4].Play();

                TalkText.text = "살아 있다니 운이 좋구만 친구! 디아블로를 반드시 찾아 저지해야만 하네! 이 일에 대해 티리엘과 이야기하게. " +
                    "그는 이 사태를 어떻게 받아들여야 할지 알고 있으니까";
            }

            if (Quest.Q > 5 && Quest.Q <= 7)
            {
                Voice[5].Play();

                TalkText.text = "자네에겐 여기서 빈둥거릴 시간 따윈 없네! 디아블로가 지옥에서 자네를 기다리네. " +
                    "기억하게… 디아블로의 최대 무기는 공포라는 것을 말이야. 자신 안의 공포에 굴복하지 말게. " +
                    "그의 힘에 맞서 그를 영원히 끝장내 버리게!";
            }

            if (Quest.Q > 7)
            {
                Voice[6].Play();

                TalkText.text = "자네에게 큰 잠재력이 있을 줄 알았네, 친구! 임무를 아주 잘 완수했다네." +
                    " 나의 조상들은 세 악마와 그의 부하들에게 맞서기 위해 노력해왔지…. 나는 고지식한 삶을 살아왔네." +
                    " 내 지식이 자네를 도울 수 있다니 기쁘군. 이제 나는 이곳을 떠나고자 하네." +
                    " 천국의 문은 겉으론 경이롭지만 난 다시는 그 문을 보고 싶지 않아. 아주 오랜 세월 동안 말이야.";
            }

            SoundCheak = false;
        }

        if (!NPCTalk.Intro && !NPCTalk.Gossip)
        {
            for (int i = 0; i < Voice.Length; i++)
                Voice[i].Stop();
        }
    }
}
