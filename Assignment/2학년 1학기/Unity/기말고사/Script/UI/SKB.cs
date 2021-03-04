using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SKB : MonoBehaviour
{

    public GameObject player;
    private PlayerState state;
    public GameObject def;   // 스킬설명UI
    public GameObject Before; // 스킬찍기 전 설명
    public GameObject After;// 스킬찍기 후 설명
    public Text SPQ;    // 해당스킬 포인트 수치
    public int SP = 0;  // 해당스킬 포인트 변수
    public int L = 1;   // 해금 레벨
    public int data = 0;// 스킬코드
    public GameObject Quick; // 단축스킬버튼
    public GameObject QuickSet; // 단축스킬위치
    public GameObject LQuick; // 단축스킬버튼
    public GameObject LQuickSet; // 단축스킬위치

    void Start()
    {       
        state = player.GetComponent<PlayerState>(); // 플레이어 스탯 호출
    }

    public void OnClick()   // 클릭했을때
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        state.skillpoint -= 1;  // 플레이어 스탯 중 스포 -1
            SP++;                   // 해당 스킬 포인트 +1
        if (SP == 1)
        {
            Quick.transform.localPosition = new Vector2(QuickSet.transform.localPosition.x - (UI_SkillManager.skx * 50), QuickSet.transform.localPosition.y + (UI_SkillManager.sky * 45));
            Quick.SetActive(true);
            UI_SkillManager.skx++;

            if (UI_SkillManager.sky == 0 && UI_SkillManager.skx == 2)
            {
                UI_SkillManager.sky++;
                UI_SkillManager.skx = 0;
            }

            if (UI_SkillManager.skx == 10)
            {
                UI_SkillManager.skx = 0;
                UI_SkillManager.sky++;
            }
            if (UI_SkillManager.lsky == 0 && UI_SkillManager.lskx == 2)
            {
                UI_SkillManager.lsky++;
                UI_SkillManager.lskx = 0;
            }

            if (UI_SkillManager.lskx == 10)
            {
                UI_SkillManager.lskx = 0;
                UI_SkillManager.lsky++;
            } 
            // 스킬 단축셋팅
            if (data > 0 )
            {
                     LQuick.transform.localPosition = new Vector2(LQuickSet.transform.localPosition.x + (UI_SkillManager.lskx * 50), LQuickSet.transform.localPosition.y + (UI_SkillManager.lsky * 45));
                     LQuick.SetActive(true);
                     UI_SkillManager.lskx++;
                             
            }
           
        }  
        
        
    }
    public void OnMEnter()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        def.SetActive(true);
        gameObject.GetComponent<Image>().color = new Color(0, 0, 200);
    }
    public void OnMExit()
    {
        def.SetActive(false);
        gameObject.GetComponent<Image>().color = new Color(255, 255, 255);
    }
    void Update()
    {
        if (state.level>=L&& state.skillpoint > 0)   // 플레이어 스포가 0보다 클때만 버튼활성화
        {
            gameObject.GetComponent<Image>().color = new Color(255,255,255);            
                gameObject.GetComponent<Button>().interactable = true;
            
                 
        }
        else 
        {
            gameObject.GetComponent<Image>().color = new Color32(100,100,100,255);
            gameObject.GetComponent<Button>().interactable = false;
        }

        if (SP > 0)
        {
            SPQ.text = "" + SP; // 해당스킬 포인트 수치 출력    
            Before.SetActive(false);
            After.SetActive(true);
        }
    }
}
