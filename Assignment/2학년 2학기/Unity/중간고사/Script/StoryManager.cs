using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

enum NPC { player , dragongirl, angel , desco = 2};

public class StoryManager : MonoBehaviour
{
    public GameObject Stage;

    public GameObject[] PrologueNPC;
    public GameObject[] EpilogueNPC;
    public GameObject[] ExtraNPC;

    public GameObject Pro, Epi, Extra;
    public Text Talk, Talk2, Talk3;

    public AudioSource BGM, BGM_Story, BGM_Epi, BGM_Extra , BGM_Extra2;

    static public bool storyreview_0 = false;
    static public bool storyreview_2 = false;
    static public bool storyreview_3 = false;

    static public int index;

    // Start is called before the first frame update
    void Start()
    {
        index = 0;
    }

    // Update is called once per frame
    void Update()
    {
        // 프롤로그
        if (PlayerPrefs.GetInt("Story") == 0 || storyreview_0 )
        {
            if (Input.GetMouseButtonDown(0) || Input.GetKeyDown(KeyCode.Space))
            {
                index += 1;
            }

            switch(index)
            {
                case 0:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    PrologueNPC[(int)NPC.player].SetActive(false);
                    PrologueNPC[(int)NPC.dragongirl].SetActive(false);

                    Talk.text = "사람의 손길이 닿지 않은 깊은 숲 속-";
                    break;

                case 1:
                    PrologueNPC[(int)NPC.player].SetActive(true);
                    Talk.text = "그러니깐 내가 왜 그딴 짓을 해야하냐고 !";
                    break;

                case 2:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    Talk.text = "두 남녀가 서로 목소리를 높이면서 언성을 벌이고 있었다.";
                    break;

                case 3:
                    PrologueNPC[(int)NPC.dragongirl].SetActive(true);
                    Talk.text = "정말 ! 어쩔 수 없잖아 ! 이 근처에 인간이라곤 너밖에 없는데 !";
                    break;

                case 4:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk.text = "하 . . 왜 하필이면 이 넓고 넓은 세상에 굳이 여기까지 와서 . .";
                    break;

                case 5:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk.text = "남자는 짙은 한숨을 내쉬면서 진심으로 한탄스러운 표정을 지었다.";
                    break;

                case 6:
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    Talk.text = "시간 없으니깐 한 번만 말할테니 잘 들어 ! 최근 정령들이 폭주하여 생태계를 파괴하고 있어."
                        + "내 힘을 빌려줄테니 폭주하고 있는 정령들을 제압해줘 ";
                    break;

                case 7:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk.text = "힘이라니. . 설마 니 등에 달린 콩알만한 날개로 하늘을 난다거나 불이라도 쏘는거야 ? " +
                        "설마 그런 생뚱 맞은 이야길 지금 나더러 믿으라는 건 아니 . .";
                    break;

                case 8:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk.text = "뭐야, 어떻게 안거야 ? 너도 설마 드래곤이었어 ?";
                    break;

                case 9:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk.text = ". . . 실화냐 ?";

                    break;

                case 10:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk.text = "[ 스페이스 ] 키를 누르면 날 수 있고, [ 마우스 왼쪽 ] 키를 누르면 기본 공격이야. ";
                    //Talk.text = "[ 왼쪽 화면 ] 을 터치하면 날 수 있어. 공격은 내가 알아서 할테니깐 어떻게든 피하기만 해주면 돼.";
                    break;

                case 11:
                    Talk.text = "그리고 마나를 일정 이상 모았을 때 [ 마우스 오른쪽 ] 키를 누르면 강력한 불을 사용할 수 있어. ";
                    //Talk.text = "그리고 마나를 일정 이상 모았을 때 [ Skill ] 버튼을 누르면 강력한 불을 사용할 수 있어. ";
                    break;

                case 12:
                    Talk.text = "굉장히 강력한 공격이지만, 마나를 많이 소모하니깐 정말 신중하게 써야해 !";
                    break;

                case 13:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk.text = "너 갑자기 누구한테 말하고 있는거야 ?";
                    break;

                case 14:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk.text = "응? 누구냐니, 이제 이 게임을 플레이 할 사람들한테 설명해주잖아";
                    break;

                case 15:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk.text = ". . 아, 그러냐";
                    break;

                case 16:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk.text = "그럼 출발하자 ! 빨리 ! 이럴 시간 없다구 !";
                    break;

                case 17:
                    PrologueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    PrologueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk.text = "하아 . . 알았다고 . . 귀찮아 죽겠네 . .";
                    break;
            }

            if (index > 17)
            {
                PlayerPrefs.SetInt("Story", 1);

                index = 0;
                storyreview_0 = false;

                BGM.Play();
                BGM_Story.Stop();

                Stage.SetActive(true);
                Pro.SetActive(false);
            }
        }

        // 에필로그
        if (storyreview_2)
        {
            if (Input.GetMouseButtonDown(0) || Input.GetKeyDown(KeyCode.Space))
            {
                index += 1;
            }

            switch(index)
            {
                case 0:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    EpilogueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    EpilogueNPC[(int)NPC.angel].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    EpilogueNPC[(int)NPC.player].SetActive(true);
                    EpilogueNPC[(int)NPC.dragongirl].SetActive(false);
                    EpilogueNPC[(int)NPC.angel].SetActive(false);

                    Talk2.text = "하아. .";
                    break;

                case 1:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = "하루 종일 날아다니는 것도 굉장히 피곤하다는 사실을 오늘 체감했다. . "
                        + "별로 알고 싶은 건 아니었지만 . .";
                    break;

                case 2:
                    EpilogueNPC[(int)NPC.dragongirl].SetActive(true);

                    Talk2.text = "설마 하루만에 이 정도로 해낼 줄이야 . . 역시 나는 대단하다니깐 ! ";
                    break;

                case 3:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    EpilogueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = ". . 아, 그러냐";
                    break;

                case 4:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = "뭐, 힘을 빌린 것은 사실이고 . . 무엇보다 말할 기력도 없다.";
                    break;

                case 5:
                    EpilogueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk2.text = ". . 뭐, 너도 조금은 칭찬해줄게. 물론 9 할 정도는 내 덕이지만";
                    break;

                case 6:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    EpilogueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = "그거 고맙구만";
                    break;

                case 7:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    EpilogueNPC[(int)NPC.dragongirl].SetActive(false);
                    EpilogueNPC[(int)NPC.angel].SetActive(true);

                    Talk2.text = "아뇨, 정말 감사하게 생각해요. 덕분에 제일 문제였던 정령들을 무사히 정화 시킬 수 있었으니까요. ";
                    break;

                case 8:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    EpilogueNPC[(int)NPC.angel].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = ". . . 누구?";
                    break;

                case 9:
                    EpilogueNPC[(int)NPC.angel].SetActive(false);
                    EpilogueNPC[(int)NPC.dragongirl].SetActive(true);

                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    EpilogueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk2.text = "선배 ?!";
                    break;

                case 10:
                    EpilogueNPC[(int)NPC.dragongirl].SetActive(false);
                    EpilogueNPC[(int)NPC.angel].SetActive(true);

                    EpilogueNPC[(int)NPC.angel].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk2.text = "이 정도로 해낼 수 있었던 것을 보니 두 분의 합이 굉장히 잘 맞으신가 보네요";
                    break;

                case 11:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    EpilogueNPC[(int)NPC.angel].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = ". . . ?";
                    break;

                case 12:
                    EpilogueNPC[(int)NPC.angel].SetActive(false);
                    EpilogueNPC[(int)NPC.dragongirl].SetActive(true);

                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    EpilogueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk2.text = "하 ?";
                    break;

                case 13:
                    EpilogueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = "무슨 말도 안되는 소리를 . . 얘랑 나랑 합이 잘 맞다고 ? 어딜 봐서 ? 아니 그것보다 . .";
                    break;

                case 14:
                    Talk2.text = "어째서인지 불길한 느낌이 든다 . .";
                    break;

                case 15:
                    EpilogueNPC[(int)NPC.dragongirl].SetActive(false);
                    EpilogueNPC[(int)NPC.angel].SetActive(true);

                    EpilogueNPC[(int)NPC.angel].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk2.text = "아직 생태계를 파괴하고 있는 정령들이 많아 걱정이 태산이었지만 . . . 두 분을 보니 조금은 " +
                        "안심이 되는군요 !";
                    break;

                case 16:
                    EpilogueNPC[(int)NPC.angel].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = "설마 . . 안돼 . . 제발 ' 앞으로도 잘 부탁해요 ' 라는 말만은 . .";
                    break;

                case 17:
                    EpilogueNPC[(int)NPC.angel].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk2.text = "앞.으.로.도 잘 부탁해요~";                
                    break;

                case 18:
                    EpilogueNPC[(int)NPC.angel].SetActive(false);
                    EpilogueNPC[(int)NPC.dragongirl].SetActive(true);

                    EpilogueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk2.text = "하 ?! 이런 음침하고 의욕 없는 남자랑 ?! 싫어 ! 절대로 싫다고 !";
                    break;

                case 19:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    EpilogueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = ". . . 거부권은요 ?";
                    break;

                case 20:
                    EpilogueNPC[(int)NPC.dragongirl].SetActive(false);
                    EpilogueNPC[(int)NPC.angel].SetActive(true);

                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    EpilogueNPC[(int)NPC.angel].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk2.text = "뭐 . . 드래곤 이상의 속도로 도망치실 수 있으시다면 말리진 않겠지만 . .";
                    break;

                case 21:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    EpilogueNPC[(int)NPC.angel].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = ". . 하아";
                    break;

                case 22:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = "남자는 짙은 한숨을 내쉬면서 진심으로 한탄스러운 표정을 지었다.";
                    break;

                case 23:
                    EpilogueNPC[(int)NPC.angel].SetActive(false);
                    EpilogueNPC[(int)NPC.dragongirl].SetActive(true);

                    EpilogueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk2.text = "싫어 ! 싫다구 ! 아직 정령이 얼마나 많이 남았는데 !";
                    break;

                case 24:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    EpilogueNPC[(int)NPC.dragongirl].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    EpilogueNPC[(int)NPC.angel].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = ". . 나도 싫다 . . 기구한 내 운명이 . .";
                    break;

                case 25:
                    EpilogueNPC[(int)NPC.player].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk2.text = "[ Epilogue End Thank you ]";
                    break;
            }

            if (index > 25)
            {
                index = 0;
                storyreview_2 = false;

                BGM.Play();
                BGM_Epi.Stop();

                Stage.SetActive(true);
                Epi.SetActive(false);
            }
        }

        if (storyreview_3)
        {
            if (Input.GetMouseButtonDown(0) || Input.GetKeyDown(KeyCode.Space))
            {
                index += 1;
            }

            switch(index)
            {
                case 0:
                    for (int i = 0; i < ExtraNPC.Length; i++) {
                        ExtraNPC[i].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                       ExtraNPC[i].SetActive(false);
                    }

                    ExtraNPC[2].SetActive(true);

                    Talk3.text = ". . . .";
                    break;

                case 1:
                    ExtraNPC[2].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = "인적이 드문 숲 속에 인간이 아닌 듯한 한 여성이 멍한 표정으로 서있었고, 그녀가 바라보고 있는 곳에는 . .";
                    break;

                case 2:
                    ExtraNPC[0].SetActive(true);

                    Talk3.text = ". . . .";
                    break;

                case 3:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = ". . 어째서인지 내가 서있었다.";
                    break;

                case 4:
                    ExtraNPC[1].SetActive(true);
                    ExtraNPC[2].SetActive(false);

                    Talk3.text = "뭐야, 아는 사이야 ?";
                    break;

                case 5:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    ExtraNPC[1].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = "그럴 리가 있겠냐 . . 애초에 쟨 어떻게 봐도 인간이 아니잖아";
                    break;

                case 6:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    ExtraNPC[1].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk3.text = "그러고보니 . .";
                    break;

                case 7:
                    ExtraNPC[1].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = "이마에는 뿔도 있고 . . 등에도 이상한 게 달려있는 것을 보아 아마도 이 아인 . .";
                    break;

                case 8:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk3.text = "너랑 같은 드래곤 아니야 ?";
                    break;

                case 9:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    ExtraNPC[1].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk3.text = "음 . . 뭔가 비슷한 냄새가 나는 것 같기도 하고 . .";
                    break;

                case 10:
                    ExtraNPC[1].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = "아까부터 계속 멍한 표정으로 멀뚱히 서있기만 했던 드래곤이 드디어 " +
                        "잠이 덜 깬듯한 가라앉은 목소리로 말했다.";
                    break;

                case 11:
                    ExtraNPC[2].SetActive(true);
                    ExtraNPC[1].SetActive(false);

                    ExtraNPC[2].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk3.text = ". . . 너희들이지 ?";
                    break;

                case 12:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    ExtraNPC[2].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = "아닌데요.";
                    break;

                case 13:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = "불길한 냄새가 코를 찌르기 시작했다. ' 저 애랑 엮이면 무조건 귀찮아질거다 . . ' 라고" +
                        " 내 육감이 말해주는 듯한 느낌이다.";
                    break;

                case 14:
                    Talk3.text = "내게 무슨 용무가 있는 지는 모르겠지만 . . 무조건 피하는 게 상책이다. " +
                        "여지껏 불길한 예감은 . . 틀린 적이 없었다.";
                    break;

                case 15:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    ExtraNPC[2].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk3.text = ". . . 아니 . . 너희들이 맞아";
                    break;

                case 16:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    ExtraNPC[2].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = "그럼 왜 묻는거야 . . .";
                    break;

                case 17:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    ExtraNPC[1].SetActive(true);
                    ExtraNPC[2].SetActive(false);

                    ExtraNPC[1].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk3.text = "뭐야, 할 말이라도 있다면 확실하게 말하라구";
                    break;

                case 18:
                    ExtraNPC[2].SetActive(true);
                    ExtraNPC[1].SetActive(false);

                    ExtraNPC[2].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk3.text = ". . . 너희들의 실적 . . 듣고 왔어";              
                    break;

                case 19:
                    ExtraNPC[2].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = "아니, 그게 그렇게까지 소문이 났다고 ? 대체 누가 . . ";
                    break;

                case 20:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk3.text = ". . . 아, 그 여자 . .";
                    break;

                case 21:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    ExtraNPC[1].SetActive(true);
                    ExtraNPC[2].SetActive(false);

                    Talk3.text = "하하핫 ! 벌써 나의 무용담이 그렇게 유명해지다니~ 역시 나야 ! ";
                    break;

                case 22:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    ExtraNPC[1].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = ". . . 하아";
                    break;

                case 23:
                    ExtraNPC[2].SetActive(true);
                    ExtraNPC[1].SetActive(false);

                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    ExtraNPC[2].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk3.text = ". . . 그래서 너희들에게 . . 부탁이 있는데 . .";
                    break;

                case 24:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    ExtraNPC[2].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = ". . . 거부권 . . 은 있을리가 없지 . .";
                    break;

                case 25:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = "이게 부탁인가 . . 명령이지 . . 정말이지 드래곤들은 지들 멋대로 하는구만 . .";
                    break;

                case 26:
                    ExtraNPC[1].SetActive(true);
                    ExtraNPC[2].SetActive(false);

                    ExtraNPC[1].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    Talk3.text = "뭐, 멀리서 내 이름을 듣고 찾아왔다면야~ 못 들어줄 것도 없지 !";
                    break;

                case 27:
                    ExtraNPC[1].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = ". . 봐, 지들 멋대로 한다는 내 말이 맞지 ?";
                    break;

                case 28:
                    ExtraNPC[2].SetActive(true);
                    ExtraNPC[1].SetActive(false);

                    ExtraNPC[2].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

                    BGM_Extra.Stop();
                    BGM_Extra2.Play();

                    Talk3.text = ". . . 그 전에 . . 실력을 . . 시험해보겠어";
                    break;

                case 29:
                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 1f);
                    ExtraNPC[2].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = ". . . 뭐? 자, 잠깐 ! 갑자기 !?";
                    break;

                case 30:
                    ExtraNPC[1].SetActive(true);
                    ExtraNPC[2].SetActive(false);

                    ExtraNPC[0].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);
                    ExtraNPC[1].GetComponent<Image>().color = new Color(1, 1, 1, 1f);

               
                    Talk3.text = "에?! 갑자기 뭐야!?";
                    break;

                case 31:
                    ExtraNPC[1].GetComponent<Image>().color = new Color(1, 1, 1, 0.5f);

                    Talk3.text = "당황할 틈도 없이 그녀는 망설이지 않고 우리에게 달려들었고, 아무래도 나는 또 다시 " +
                        "귀찮은 일에 휘말린 모양이다 . .";
                    break;
            }

            if (index > 31)
            {
                PlayerPrefs.SetInt("Story", 4);

                index = 0;
                storyreview_3 = false;

                BGM.Play();
                BGM_Extra2.Stop();

                Stage.SetActive(true);
                Extra.SetActive(false);
            }
        }
    }
}
