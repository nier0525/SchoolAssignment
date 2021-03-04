using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI_SkillManager : MonoBehaviour
{

    public GameObject player;
    private PlayerState state;
    public Text SPQ;
    public Text Skill_Blizzard_T;
    public Text Skill_FireBall_T;
    public Text Skill_FireWall_T;
    public Text Skill_Blaze_T;
    public Text Skill_FireBalt_T;
    public Text Skill_Meteo_T;
    public Text Skill_EnergyShield_T;
    public Text Skill_FrozenOve_T;

    public GameObject Biz; // 블리자드 
    public GameObject FiB; // 파이어볼
    public GameObject FiW; // 파이어월
    public GameObject Blz; // 블레이즈
    public GameObject Fit; // 파이어볼트
    public GameObject MET; // 메테오
    public GameObject Ene; // 에너지쉴드
    public GameObject Fov; // 프로즌오브

    public static int skx = 0;
    public static int lskx = 0;
    public static int lsky = 0;
    public static int sky = 0;
    //스킬단축위치


    private void Start()
    {
        state = player.GetComponent<PlayerState>();
        
    }
    private void Update()
    {
        // 데미지 설정
        blizzard.min_damage = 15 * (2 + Biz.GetComponent<SKB>().SP);
        blizzard.max_damage = 15 * (2 + Biz.GetComponent<SKB>().SP) + 29 + Biz.GetComponent<SKB>().SP;

        icestorm.min_damage = 8 * (2 * Biz.GetComponent<SKB>().SP);
        icestorm.max_damage = 15 * (2 + Biz.GetComponent<SKB>().SP) + 29 + Biz.GetComponent<SKB>().SP;

        fireball.min_damage = FiB.GetComponent<SKB>().SP * 8 - 1 
            + (int)((double)(Fit.GetComponent<SKB>().SP) * (double)(FiB.GetComponent<SKB>().SP * 8 - 1) * 0.16)
            + (int)((double)(MET.GetComponent<SKB>().SP) * (double)(FiB.GetComponent<SKB>().SP * 8 - 1) * 0.16); 
        fireball.max_damage = (FiB.GetComponent<SKB>().SP + 1) * 9 - 1
             + (int)((double)(Fit.GetComponent<SKB>().SP) * (double)(FiB.GetComponent<SKB>().SP * 9 - 1) * 0.16)
            + (int)((double)(MET.GetComponent<SKB>().SP) * (double)(FiB.GetComponent<SKB>().SP * 9 - 1) * 0.16); 

        firewall.min_damage = FiW.GetComponent<SKB>().SP * 52 + 34;
        firewall.max_damage = FiW.GetComponent<SKB>().SP * 52 + 63;

        firebolt.min_damage = Fit.GetComponent<SKB>().SP * 2 + 1
            +(int)((double)(FiB.GetComponent<SKB>().SP)*(double)(Fit.GetComponent<SKB>().SP * 2 + 1)*0.16)
            +(int)((double)(MET.GetComponent<SKB>().SP) * (double)(Fit.GetComponent<SKB>().SP * 2 + 1) * 0.16);
        firebolt.max_damage = Fit.GetComponent<SKB>().SP * 2 + 5 
            + (int)((double)(FiB.GetComponent<SKB>().SP) * (double)(Fit.GetComponent<SKB>().SP * 2 + 5)*0.16 )
            + (int)((double)(MET.GetComponent<SKB>().SP) * (double)(Fit.GetComponent<SKB>().SP * 2 + 5) * 0.16);

        hydragroundfire.min_damage = 10 + Blz.GetComponent<SKB>().SP * 12;
        hydragroundfire.max_damage = 33 + Blz.GetComponent<SKB>().SP * 13;

        meteor.min_damage = 70 + MET.GetComponent<SKB>().SP * 3
             + (int)((double)(Fit.GetComponent<SKB>().SP) * (double)(70 + MET.GetComponent<SKB>().SP * 3) * 0.16)
            + (int)((double)(FiB.GetComponent<SKB>().SP) * (double)(70 + MET.GetComponent<SKB>().SP * 3) * 0.16);
        meteor.max_damage = 93 + MET.GetComponent<SKB>().SP * 3 + MET.GetComponent<SKB>().SP
            + (int)((double)(Fit.GetComponent<SKB>().SP) * (double)(93 + MET.GetComponent<SKB>().SP * 3 + MET.GetComponent<SKB>().SP) * 0.16)
            + (int)((double)(FiB.GetComponent<SKB>().SP) * (double)(93 + MET.GetComponent<SKB>().SP * 3 + MET.GetComponent<SKB>().SP) * 0.16);

        m_fire.min_damage = 10 + MET.GetComponent<SKB>().SP * 2;
        m_fire.max_damage = 33 + MET.GetComponent<SKB>().SP * 2;

        iceorb.min_damage = (3 + Fov.GetComponent<SKB>().SP) * 10;
        iceorb.max_damage = (3 + Fov.GetComponent<SKB>().SP) * 11;

        iceboltController.damage = (3 + Fov.GetComponent<SKB>().SP) * 5;
        //


        // 텍스트 입력
        SPQ.text = "" + state.skillpoint;
        Skill_Blizzard_T.text = "현재 스킬 : " + Biz.GetComponent<SKB>().SP +
            "\n" + "콜드 데미지 : " + blizzard.min_damage + "-" + blizzard.max_damage + "\n" + "지속시간 : 4초\n" +
            "마나 소모량 : 23";
        Skill_FireBall_T.text = "반경 : 1 야드\n" + "현재 스킬 : " + FiB.GetComponent<SKB>().SP + "\n" +
            "파이어 데미지 : " + fireball.min_damage + "-" + fireball.max_damage + "\n" + "마나 소모량 : 5";
        Skill_FireWall_T.text = "현재 스킬 : " + FiW.GetComponent<SKB>().SP + "\n" +
            "파이어 지속시간 : 3.6 초\n" + "평균 파이어 데미지 : " + firewall.min_damage + "-"
            + firewall.max_damage + " per second\n" + "4.6 야드\n" + "마나 소모량 : 22";
        Skill_FireBalt_T.text = "마나 소모량 : 2.5\n\n" + "현재 스킬 : " + Fit.GetComponent<SKB>().SP +
            "\n" + "파이어 데미지 : " + firebolt.min_damage + "-" + firebolt.max_damage;
        Skill_Blaze_T.text = "현재 스킬 : " + Blz.GetComponent<SKB>().SP + "\n" +
            "파이어 지속시간: 3.6 초\n" + "평균 파이어 데미지 : " + hydragroundfire.min_damage + "-" +
            hydragroundfire.max_damage + " per second \n" + "마나 소모량 : 11";
        Skill_Meteo_T.text = "현재 스킬 : " + MET.GetComponent<SKB>().SP + "\n" +
            "파이어 데미지 : " + m_fire.min_damage + "-" + m_fire.max_damage + "\n" +
            "반경 : 4 야드\n" + "평균 파이어 데미지 : " + (32 + 11 * MET.GetComponent<SKB>().SP) +
            "-" + (62 + 10 * MET.GetComponent<SKB>().SP) + "\n" + "마나 소모량 : 18";
        Skill_FrozenOve_T.text = "현재 스킬 : " + Fov.GetComponent<SKB>().SP + "\n" +
            "콜드 데미지 : " + iceorb.min_damage + "-" + iceorb.max_damage + "\n" +
            "둔화 지속 : 8 초\n" + "마나 소모량 : 25";
   

        //
    }





}
