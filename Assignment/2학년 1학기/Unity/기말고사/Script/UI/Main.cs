using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


// 런
// 경험치
// 라이프
// 마나
// 스킬금지

public class Main : MonoBehaviour
{
    public GameObject player;
    private PlayerState state;
    public Text Life_Text;
    public Text Mana_Text;
    private string LF;//라이프
    private string MN;//마나
    void Start()
    {
        LF = "라이프";
        MN = "마나";
        //player = GameObject.Find("player"); // 플레이어 오브젝트 찾기
        state = player.GetComponent<PlayerState>(); // 플레이어 스탯 호출
    }

    void Update()
    {
        Life_Text.text = LF + " : " + state.hp + " / " + state.Max_hp;
        Mana_Text.text = MN + " : " + state.mp + " / " + state.Max_mp;

        ////if (Input.GetKeyUp(KeyCode.L))
        ////{
        ////    state.LevelUp();
        ////}
    }
    

}
