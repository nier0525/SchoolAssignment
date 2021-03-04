using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class QuestTalk : MonoBehaviour
{
    public GameObject Talk, Player;
    public Text TalkText;
    public AudioSource[] voice;

    public bool mousedown = false;

    // Start is called before the first frame update
    void Start()
    {
        Talk.SetActive(false);
    }

    IEnumerator WaitMouseDown()
    {
        yield return new WaitForSeconds(2f);
        mousedown = true;
    }

    // Update is called once per frame
    void Update()
    {
       if(triel.Enter|| IzualSoul.Enter || hadrial.Enter || cain.Enter)
            TalkText.GetComponent<RectTransform>().localPosition += new Vector3(0, 0.2f, 0);
       else
            TalkText.GetComponent<RectTransform>().localPosition = new Vector3(-1.26f, -240.4f, 0);

        if (Quest.Q == 0 && triel.Enter)
        {
            voice[0].Play();
            TalkText.text = "이 버려진 영역에는 오래 전에 봉인된 사악하고 고통받는 영혼이 있소. " +
                "인간들은 그를 이주얼이라고 불렀으며 먼 과거에 그는 내가 가장 신뢰하는 부관이었소." +
                " 하지만 그는 내 명령을 어기고 작열하는 헬포지에 습격을 감행했다가 실패했소." +
                " 그의 용맹과 힘에도 불구하고 이주얼은 대악마들에게 사로잡혔고 그들의 사악한 힘에 의해 타락해버렸소." +
                " 그들은 그로 하여금 동족을 배신하고 천계의 가장 중대한 비밀을 털어놓도록 강요했소." +
                " 그래서 그는 천계와 지옥 모두에게 불신 받는 타락천사가 되어버리고 말았소." +
                " 그가 지은 죄의 대가로 이주얼의 영혼은 심연으로부터 소환된 끔찍한 괴물의 형체 안에 갇히게 되었소. " +
                "그의 미쳐버린 영혼은 그 고통스러운 껍질 안에서 이미 충분히 고통을 받은 것 같소. 영웅이여, 그대에게 간청하겠소. " +
                "이주얼을 찾아 그를 잔인한 감옥으로부터 해방시켜 주시오. 그의 죄책감과 고통에 종지부를 찍으시오.";

            Player.GetComponent<PlayerControl>().Stop();
            Player.GetComponent<PlayerControl>().Wait();
            Talk.SetActive(true);

            StartCoroutine(WaitMouseDown());
            Quest.Next_Quest();
        }

        if (Quest.Q == 2 && IzualSoul.Enter)
        {
            
            voice[1].Play();
            TalkText.text = "날 믿었다니 티리엘은 얼간이야!" +
                " 이봐, 디아블로와 그의 형제들에게 소울 스톤과 그것을 사용하는 방법에 대해 알려준 건 바로 나야." +
                " 대악마들이 스스로를 너희 세계로 추방하는 음모를 꾸며 실행하도록 도운 것도 나다." +
                " 우리가 아주 먼 옛날 실행했던 계획은 어떠한 죽음의 힘으로도 막을 수 없지. " +
                "지옥은 피와 악몽의 물결처럼 너희 세계를 장악할 준비가 되어 있다." +
                " 너와 너의 종족들은 파멸될 것이다.";

            Player.GetComponent<PlayerControl>().Stop();
            Player.GetComponent<PlayerControl>().Wait();
            Talk.SetActive(true);

            StartCoroutine(WaitMouseDown());
            Quest.Next_Quest();
        }

        if (Quest.Q == 3 && hadrial.Enter)
        {
            voice[2].Play();
            TalkText.text = "멈추시오! 디아블로의 본거지로 가려면 헬포지로 가서 헤파스토를 처치하시오." +
                "불의 장막을 없애기 위해선 그 놈이 가진 열쇠가 필요하니 그것을 가지고 오시오.";

            Player.GetComponent<PlayerControl>().Stop();
            Player.GetComponent<PlayerControl>().Wait();
            Talk.SetActive(true);

            StartCoroutine(WaitMouseDown());
            Quest.Next_Quest();
        }

        if (Quest.Q == 5 && hadrial.Enter)
        {
            voice[3].Play();
            TalkText.text = "영웅이여, 디아블로의 소굴로 나아가시오. 그대는 디아블로와 최종 결전을 위해서는 " +
                "디아블로의 본거지 깊은 곳에 있는 세가지의 봉인을 풀어야 한다는 사실을 알아야만 하오." +
                "각 봉인을 제거하고, 디아블로를 처치하시오.";

            Player.GetComponent<PlayerControl>().Stop();
            Player.GetComponent<PlayerControl>().Wait();
            Talk.SetActive(true);

            StartCoroutine(WaitMouseDown());
            Quest.Next_Quest();
        }

        if ((Quest.Q == 7 || triel.Last) && triel.Enter)
        {
            voice[4].Play();
            TalkText.text = "빛을 찬양하라! 당신은 불가능한 일을 성취했소! 디아블로는 그를 집어삼킬 검은 심연으로 추방되어 버렸고," +
                "소울 스톤도 더 이상 더럽혀지지 않게 되었소. 잘했소. 우리의 영웅이여! 이제 당신이 마음껏 기뻐할 시간이오.";

            Player.GetComponent<PlayerControl>().Stop();
            Player.GetComponent<PlayerControl>().Wait();
            Talk.SetActive(true);
            triel.Last = false;

            if (Quest.Q == 7)
            {
                StartCoroutine(WaitMouseDown());
                Quest.Next_Quest();
            }
        }

        if (Input.GetMouseButtonDown(0) && !NPCTalk.Intro && !NPCTalk.Gossip && mousedown)
        {
            for (int i = 0; i < voice.Length; i++)
                voice[i].Stop();
            Talk.SetActive(false);
            if (IzualSoul.Enter)
                IzualSoul.clear = false;

            Player.GetComponent<PlayerControl>().Play();
            mousedown = false;
        }
    }
}
