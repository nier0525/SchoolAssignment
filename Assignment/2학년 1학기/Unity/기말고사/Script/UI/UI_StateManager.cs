using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI_StateManager : MonoBehaviour
{
    public GameObject player;
    public GameObject RUN;
    public GameObject WALK;
    private PlayerState state;
    private PlayerMotion motion;
    private SkillDatabase skdata;

    public Text LvQ;  // 레벨 수치
    public Text ExpQ;  // 경험치 수치
    public Text NLEQ;  // 다음레벨 수치
    public Text strQ;  // 힘 수치
    public Text dexQ;  // 민첩 수치
    public Text helQ;  // 생명력 수치
    public Text eneQ;  // 에너지 수치
    public Text SK1;  // 1스킬 수치
    public Text SK2;  // 2스킬 수치
    public Text GuaQ;  // 방어 수치
    public Text STM;  // 스테미나 최대
    public Text STQ;  // 스테미나 현재
    public Text LFQ;  // 라이프 현재
    public Text LFM;  // 라이프 최대
    public Text MM;  // 마나 최대
    public Text MQ;  // 마나 현재
    public Text FQ;  // 파이어 저항력
    public Text CQ;  // 콜드 저항력
    public Text LQ;  // 라이트닝 저항력
    public Text PQ;  // 포이즌 저항력
    public Text SP; // 스포
    public Text SK1Q;
    public Text SK2Q;
    //
    public GameObject strUp;
    public GameObject dexUp;
    public GameObject helUp;
    public GameObject eneUp;
    // 스텟업 버튼

    //게이지들
    public GameObject HP;
    public GameObject MP;
    public GameObject Run;
    public GameObject Exp;
    //
    public GameObject RSK;
    public GameObject LSK;
    public  int ld;
    public  int rd;
    public int ldd;
    public int rdd;

    public static bool Rbutton = false;

    private void Start()
    {
        state = player.GetComponent<PlayerState>();
        motion = player.GetComponent<PlayerMotion>();
    } // 스크립트 상속

    public void strclick()  //힘버튼
    {
        state.plusStr();
        Debug.Log("str");
    }
    public void dexclick()  //민첩버튼
    {
        state.plusDex();
        Debug.Log("del");
    }
    public void helclick() // 생명력버튼
    {
        state.plusLife();
        Debug.Log("hel");
       
    }
    public void eneclick() // 에너지버튼
    {
        state.plusEnergy();
        Debug.Log("ene");
    }
   
    private void Update()
    {
        ldd = 20;
        rdd = 20;
        switch (state.L_Skill)
        {
            case 1:
                ld = blizzard.max_damage;
                break;
            case 2:
                ld = iceorb.max_damage;
                break;
            case 3:
                ld = manashield.max_damage;
                break;
            case 4:
                ld = hydragroundfire.max_damage;
                break;
            case 5:
                ld = firewall.max_damage;
                break;
            case 6:
                ld = firebolt.max_damage;
                break;
            case 7:
                ld = fireball.max_damage;
                break;
            case 8:
                ld = m_fire.max_damage;
                break;
            default:
                ld = 0;
                ldd = 0;
                break;

        }
        switch (state.R_Skill)
        {
            case 1:
                rd = blizzard.max_damage;
                break;
            case 2:
                rd = iceorb.max_damage;
                break;
            case 3:
                rd = manashield.max_damage;
                break;
            case 4:
                rd = hydragroundfire.max_damage;
                break;
            case 5:
                rd = firewall.max_damage;
                break;
            case 6:
                rd = firebolt.max_damage;
                break;
            case 7:
                rd = fireball.max_damage;
                break;
            case 8:
                rd = m_fire.max_damage;
                break;
            default:
                rd = 0;
                rdd = 0;
                break;
        }
        




        LvQ.text = ""+state.level;
        ExpQ.text = "" + state.exp;
        NLEQ.text = "" + state.nextexp;
        strQ.text = "" + state.str;
        dexQ.text = "" + state.dex;
        helQ.text = "" + state.life;
        eneQ.text = "" + state.energy;

        SK1.text = "" + LSK.gameObject.name;// 스킬왼쪽단축칸
        SK2.text = "" + RSK.gameObject.name;// 스킬오른쪽단축칸
        SK1Q.text = ld + "-" +(ld+ldd);
        SK2Q.text = rd + "-" + (rd+rdd);



        GuaQ.text = "" + state.Guard;// 가드

        STM.text = "" + state.Max_stamina;
        STQ.text = "" + (int)state.stamina;
        LFM.text = "" + state.Max_hp;
        LFQ.text = "" + state.hp;
        MM.text = "" + state.Max_mp;
        MQ.text = "" + state.mp;
        FQ.text = "" + state.fire_resistance;
        CQ.text = "" + state.cold_resistance;
        LQ.text = "" + state.lightning_resistance;
        PQ.text = "" + state.poison_resistance;
        SP.text = "" + state.statepoint;


        HP.GetComponent<Image>().fillAmount = (float)state.hp / (float)state.Max_hp;
        MP.GetComponent<Image>().fillAmount = (float)state.mp / (float)state.Max_mp;
        Run.GetComponent<Image>().fillAmount = (float)state.stamina / (float)state.Max_stamina;
        Exp.GetComponent<Image>().fillAmount = (float)state.exp / (float)state.nextexp;

        if (state.statepoint > 0) // 스텟포인트 0이상일때 버튼 활성화
        {
            strUp.SetActive(true);
            dexUp.SetActive(true);
            helUp.SetActive(true);
            eneUp.SetActive(true);
        }
        else
        {
            strUp.SetActive(false);
            dexUp.SetActive(false);
            helUp.SetActive(false);
            eneUp.SetActive(false);
        }

        if (!Rbutton)
        {
            //
            if (Input.GetKey(KeyCode.LeftControl) && !Input.GetKey(KeyCode.LeftShift))
            {
                RUN.SetActive(true);
                WALK.SetActive(false);

                if (state.stamina > 0)
                    state.stamina -= 0.7f;
                else
                    motion.isRun = false;

            }
            else
            {
                if ((state.stamina < state.Max_stamina))
                    state.stamina += 1.5f;
            }
            // 스태미나 

            if (Input.GetKeyUp(KeyCode.LeftControl))
            {
                RUN.SetActive(false);
                WALK.SetActive(true);
            }
        }

        if (Input.GetKeyDown(KeyCode.R))
        {
            if (RUN.active) // 해제
            {
                Rbutton = false;

                RUN.SetActive(false);
                WALK.SetActive(true);
            }
            else            // 유지
            {
                Rbutton = true;

                RUN.SetActive(true);
                WALK.SetActive(false);
            }
        }

        if (Rbutton)
        {
            if (state.stamina > 0 && motion.isRun)
                state.stamina -= 0.7f;

            if (state.stamina < state.Max_stamina && !motion.isRun && !motion.isWalk)
                state.stamina += 1.5f;

            if (state.stamina <= 0)
            {
                motion.isWalk = true;

                //RUN.SetActive(false);
                //WALK.SetActive(true);

                //Rbutton = false;
            }
        }

    }


}
