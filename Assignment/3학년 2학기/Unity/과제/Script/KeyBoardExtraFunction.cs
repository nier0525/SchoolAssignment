using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

// 키보드 타입 열거형 값
public enum KEYBOARDTYPE : int
{ 
    SMALL_ENG = 0,
    BIG_ENG,
    SMALL_KOR,
    BIG_KOR,
}

public class KeyBoardExtraFunction : MonoBehaviour
{
    // 문자열 조립, 분해에 참조 되는 값
    static string ChoSungTBL = "ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ";
    static string JungSungTBL = "ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ";
    static string JongSungTBL = " ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅅㅆㅇㅈㅊㅋㅌㅍㅎ";

    
    string ExtraChoSung = "ㄳㄵㄶㄺㄻㄼㄽㄾㄿㅀㅄ";
    string ExtraJungSung = "ㅘㅙㅚㅝㅞㅟㅢ";

    static ushort UniCodeHangulBase = 0xAC00;
    static ushort UnicodeHangeulLast = 0xD79F;    

    // 초성, 중성 검사
    bool ChoSung_Check(string str)
    {        
        foreach (var item in ChoSungTBL)
        {
            if (item.ToString() == str)
                return true;
        }
        return false;
    }
    bool JungSung_Check(string str)
    {
        foreach (var item in JungSungTBL)
        {
            if (item.ToString() == str)
                return true;
        }
        return false;
    }

    // ㄳ,ㄶ . . .
    // ㅘ, ㅢ . . .
    // 같은 문자 검사
    bool ExtraChosung_Check(string str)
    {
        foreach (var item in ExtraChoSung)
        {
            if (item.ToString() == str)
                return true;
        }
        return false;
    }

    bool ExtraJungSung_Check(string str)
    {
        foreach (var item in ExtraJungSung)
        {
            if (item.ToString() == str)
                return true;
        }
        return false;
    }

    // 초성, 중성, 종성 조합
    static char Merge_Kor(string chosung, string jungsung, string jongsung = " ")
    {
        //print(chosung + jungsung + jongsung);

        int cho_pos = 0;
        int jung_pos = 0;
        int jong_pos = 0;

        // 받은 문자의 위치값을 찾는다.
        foreach (var str in ChoSungTBL)
        {
            if (str.ToString() == chosung)
            {
                break;
            }

            cho_pos++;
        }

        foreach (var str in JungSungTBL)
        {
            if (str.ToString() == jungsung)
                break;

            jung_pos++;
        }

        foreach (var str in JongSungTBL)
        {
            if (str.ToString() == jongsung)
                break;

            jong_pos++;
        }

        // 유니코드 한글 조합 식 ( 0xAC00 + ( 초성 * 21 + 중성) * 28 + 종성 )
        int nUnicode = UniCodeHangulBase + (cho_pos * 21 + jung_pos) * 28 + jong_pos;
        return Convert.ToChar(nUnicode);
    }

    // 초성, 중성, 종성 분리
    static string Divide_Kor(char str)
    {
        // 유니코드 값 얻기
        ushort check = Convert.ToUInt16(str);         
        int Code = check - UniCodeHangulBase;
        
        // 각각 초성, 중성, 종성 값 얻기
        int JongsungCode = Code % 28;
        Code = (Code - JongsungCode) / 28;

        int JungsungCode = Code % 21;
        Code = (Code - JungsungCode) / 21;

        int ChosungCode = Code;
        
        var chosung = ChoSungTBL[ChosungCode];
        var jungsung = JungSungTBL[JungsungCode];
        var jongsung = JongSungTBL[JongsungCode];

        string result = chosung.ToString() + jungsung.ToString() + jongsung.ToString();
        return result;
    }

    // ㄳ, ㄶ . . . 같은 문자열 조합
    string Merge_Chosung_Extra(string prev, string current)
    {
        //ㄳ
        if (prev == "ㄱ" && current == "ㅅ")
            return "ㄳ";
        
        //ㄵ
        if (prev == "ㄴ" && current == "ㅈ")
            return "ㄵ";

        //ㄶ
        if (prev == "ㄴ" && current == "ㅎ")
            return "ㄶ";

        //ㄺ
        if (prev == "ㄹ" && current == "ㄱ")
            return "ㄺ";

        //ㄻ
        if (prev == "ㄹ" && current == "ㅁ")
            return "ㄻ";

        //ㄼ
        if (prev == "ㄹ" && current == "ㅂ")
            return "ㄼ";

        //ㄽ
        if (prev == "ㄹ" && current == "ㅅ")
            return "ㄽ";

        //ㄾ
        if (prev == "ㄹ" && current == "ㅌ")
            return "ㄾ";

        //ㄿ
        if (prev == "ㄹ" && current == "ㅍ")
            return "ㄿ";

        //ㅀ
        if (prev == "ㄹ" && current == "ㅎ")
            return "ㅀ";

        //ㅄ
        if (prev == "ㅂ" && current == "ㅅ")
            return "ㅄ";

        return string.Empty;
    }

    // ㄳ, ㄶ . . . 같은 문자열 분리
    void Divide_Chosung_Extra(string str, out string s1, out string s2)
    {        
        //ㄳ
        if (str == "ㄳ")
        {
            s1 = "ㄱ";
            s2 = "ㅅ";
            return;
        }

        //ㄵ
        if (str == "ㄵ")
        {
            s1 = "ㄴ";
            s2 = "ㅈ";
            return;
        }

        //ㄶ
        if (str == "ㄶ")
        {
            s1 = "ㄴ";
            s2 = "ㅎ";
            return;
        }

        //ㄺ
        if (str == "ㄺ")
        {
            s1 = "ㄹ";
            s2 = "ㄱ";
            return;
        }

        //ㄻ
        if (str == "ㄻ")
        {
            s1 = "ㄹ";
            s2 = "ㅁ";
            return;
        }

        //ㄼ
        if (str == "ㄼ")
        {
            s1 = "ㄹ";
            s2 = "ㅂ";
            return;
        }

        //ㄽ
        if (str == "ㄽ")
        {
            s1 = "ㄹ";
            s2 = "ㅅ";
            return;
        }

        //ㄾ
        if (str == "ㄾ")
        {
            s1 = "ㄹ";
            s2 = "ㅌ";
            return;
        }

        //ㄿ
        if (str == "ㄿ")
        {
            s1 = "ㄹ";
            s2 = "ㅍ";
            return;
        }

        //ㅀ
        if (str == "ㅀ")
        {
            s1 = "ㄹ";
            s2 = "ㅎ";
            return;
        }

        //ㅄ
        if (str == "ㅄ")
        {
            s1 = "ㅂ";
            s2 = "ㅅ";
            return;
        }

        s1 = s2 = string.Empty;
    }

    // ㅘ, ㅚ . . . 같은 문자열 조합
    string Merge_Jungsung_Extra(string prev, string current)
    {
        // ㅘ
        if(prev == "ㅗ" && current == "ㅏ")
            return "ㅘ";

        // ㅙ
        if (prev == "ㅗ" && current == "ㅐ")
            return "ㅙ";

        // ㅚ
        if (prev == "ㅗ" && current == "ㅣ")
            return "ㅚ";

        // ㅝ
        if (prev == "ㅜ" && current == "ㅓ")
            return "ㅝ";

        // ㅞ
        if (prev == "ㅜ" && current == "ㅔ")
            return "ㅞ";

        // ㅟ
        if (prev == "ㅜ" && current == "ㅣ")
            return "ㅟ";

        // ㅢ
        if (prev == "ㅡ" && current == "ㅣ")
            return "ㅢ";

        return string.Empty;
    }    

    // 변수
    public InputField input;
    public KEYBOARDTYPE currentType;

    [HideInInspector] public RectTransform m_Content;
    [HideInInspector] public Text m_TextProtoType;

    [HideInInspector] public GameObject[] m_KeyboardType;    
    Dictionary<KEYBOARDTYPE, GameObject> KeyboardType;

    private void Awake()
    {
        KeyboardType = new Dictionary<KEYBOARDTYPE, GameObject>();

        KeyboardType.Add(KEYBOARDTYPE.SMALL_ENG, m_KeyboardType[0]);
        KeyboardType.Add(KEYBOARDTYPE.BIG_ENG, m_KeyboardType[1]);
        KeyboardType.Add(KEYBOARDTYPE.SMALL_KOR, m_KeyboardType[2]);
        KeyboardType.Add(KEYBOARDTYPE.BIG_KOR, m_KeyboardType[3]);

        KeyboardType[KEYBOARDTYPE.SMALL_ENG].SetActive(false);
        KeyboardType[KEYBOARDTYPE.BIG_ENG].SetActive(false);
        KeyboardType[KEYBOARDTYPE.SMALL_KOR].SetActive(false);
        KeyboardType[KEYBOARDTYPE.BIG_KOR].SetActive(false);

        KeyboardType[currentType].SetActive(true);
    }

    // 영어, 숫자 입력
    public void NotKor_Text()
    {
        // 현재 선택 된 오브젝트 확인
        var obj = EventSystem.current.currentSelectedGameObject;

        // 오브젝트 이름 ( 값 ) 입력
        input.text += obj.name;
    }

    // 한글 입력
    public void Kor_Text()
    {
        // 현재 선택 된 오브젝트 확인
        var obj = EventSystem.current.currentSelectedGameObject;

        try
        {
            // 마지막 문자 읽기
            var prev_str = input.text[input.text.Length - 1];

            // 마지막 문자가 영문인가?            
            // 마지막 문자가 ㅘ, ㅚ . . . 형식인가?
            if (char.GetUnicodeCategory(prev_str) != System.Globalization.UnicodeCategory.OtherLetter || ExtraJungSung_Check(prev_str.ToString()))
            {
                // 맞다면 입력하고 글자가 더 이상 조합되지 않는 경우임으로 함수 종료
                input.text += obj.name;
                return;
            }

            // 현재 입력 할 문자 읽기
            var current_str = obj.name;

            // 두 문자 모두 초성인 경우
            if (ChoSung_Check(prev_str.ToString()) && ChoSung_Check(current_str.ToString()))
            {
                // ㄳ, ㄶ . . . 문자 조합
                var result = Merge_Chosung_Extra(prev_str.ToString(), current_str);

                // 조합이 되었다면
                if (result != string.Empty)
                {
                    // 마지막 문자 제거 후,
                    Push_Back();
                    // 조합된 문자 입력
                    input.text += result;
                }
                else
                    // 조합이 불가능하다면 선택된 값 바로 입력
                    input.text += obj.name;
                return;
            }

            // 두 문자 모두 중성인 경우
            if (JungSung_Check(prev_str.ToString()) && JungSung_Check(current_str.ToString()))
            {
                // ㅘ, ㅚ . . . 같은 문자 조합
                var result = Merge_Jungsung_Extra(prev_str.ToString(), current_str);
                if (result != string.Empty)
                {
                    // 마지막 문자 제거
                    Push_Back();
                    // 조합된 문자 입력
                    input.text += result;
                }
                else
                    // 선택한 문자 입력
                    input.text += obj.name;
                return;
            }

            // 이전 문자가 중성만 있는 경우
            if (JungSung_Check(prev_str.ToString()))
            {
                // 조합이 불가능함으로 선택한 값만 입력 후 종료
                input.text += obj.name;
                return;
            }

            // 이전 문자가 초성만 있는 경우
            if (ChoSung_Check(prev_str.ToString()))
            {
                // 마지막 값 제거
                Push_Back();
                // 초성 + 중성 조합
                var merge = Merge_Kor(prev_str.ToString(), current_str);
                // 조합된 값 입력
                input.text += merge.ToString();
                return;
            }

            // 마지막 문자가 ㄳ, ㄶ . . . 형식인가?
            if (ExtraChosung_Check(prev_str.ToString()))
            {
                string s1, s2;
                Divide_Chosung_Extra(prev_str.ToString(), out s1, out s2);

                Push_Back();
                input.text += s1;

                var merge = Merge_Kor(s2, current_str);
                input.text += merge;
                return;
            }

            // 이전 문자 한글 분리
            // divide[0] = 초성 , divide[1] = 중성, divide[2] = 종성
            var divide = Divide_Kor(prev_str);

            // 받침이 없을 때
            if (divide[2].ToString() == " ")
            {
                // 현재 문자가 중성자인 경우
                if (JungSung_Check(current_str))
                {
                    // 이전 문자에 중성자가 조합이 가능한 경우
                    var result = Merge_Jungsung_Extra(divide[1].ToString(), current_str);
                    if (result != string.Empty)
                    {
                        // 마지막 문자 제거
                        Push_Back();
                        // 초성과 조합된 중성을 조합
                        var merge = Merge_Kor(divide[0].ToString(), result);
                        // 조합 된 값 입력
                        input.text += merge.ToString();
                        return;
                    }
                    else
                        // 선택된 값 입력
                        input.text += obj.name;
                    return;
                }
                else
                {
                    // 마지막 문자 제거
                    Push_Back();
                    // 초성 + 중성 + 현재 선택된 값을 받침으로 조합
                    var merge = Merge_Kor(divide[0].ToString(), divide[1].ToString(), current_str);
                    // 조합된 값 입력
                    input.text += merge.ToString();
                    return;
                }
            }
            // 받침이 있을 때
            else
            {
                // 현재 값이 초성인 경우
                if (ChoSung_Check(current_str))
                {
                    // 받침과 ㄳ, ㄶ . . . 조합이 가능한가?
                    var result = Merge_Chosung_Extra(divide[2].ToString(), current_str);
                    if (result != string.Empty)
                    {
                        // 가능하다면 마지막 문자 제거 후,
                        // 초성 + 중성 + 조합 된 값 조합
                        Push_Back();
                        var merge = Merge_Kor(divide[0].ToString(), divide[1].ToString(), result);
                        input.text += merge.ToString();
                    }
                    else
                        // 불가능하다면 선택된 값 입력
                        input.text += current_str;

                    return;
                }          
                // 선택된 값이 중성인 경우
                else if (JungSung_Check(current_str))
                {
                    // 받침이 ㄳ, ㄶ . . . 문자인 지 검사
                    if (ExtraChosung_Check(divide[2].ToString()))
                    {
                        // 만약 받침이 ㄳ 이라고 가정한다면
                        // divide[2] = ㄳ
                        // s1 = ㄱ , s2 = ㅅ
                        // 와 같은 방식으로 분리
                        string s1, s2;
                        Divide_Chosung_Extra(divide[2].ToString(), out s1, out s2);
  
                        // 마지막 문자 제거
                        Push_Back();
                        // 초성 + 중성 + 분리 된 초성 조합
                        var merge = Merge_Kor(divide[0].ToString(), divide[1].ToString(), s1);
                        // 조합된 값 입력
                        input.text += merge.ToString();

                        // 분리 된 나머지 초성 + 현재 선택한 중성 조합
                        merge = Merge_Kor(s2, current_str);
                        // 조합된 값 입력
                        input.text += merge.ToString();
                        return;
                    }
                    // ㄳ, ㄶ . . . 와 같은 문자가 아니라면
                    else
                    {
                        // 마지막 문자 제거
                        Push_Back();
                        // 초성 + 중성 조합
                        var merge = Merge_Kor(divide[0].ToString(), divide[1].ToString());
                        input.text += merge.ToString();

                        // 받침 + 선태한 중성 조합
                        merge = Merge_Kor(divide[2].ToString(), current_str);
                        input.text += merge.ToString();
                        return;
                    }
                }
            }
        }
        catch
        {
            // 아무 문자가 없는 경우
            if (input.text == string.Empty)
                input.text += obj.name;
        }
    }

    // 소문자, 대문자 전환
    public void Push_Shift()
    {
        // 현재 키보드 상태에 따라 오브젝트 변경
        KeyboardType[currentType].SetActive(false);

        switch (currentType)
        {
            case KEYBOARDTYPE.SMALL_ENG:
                currentType = KEYBOARDTYPE.BIG_ENG;
                break;

            case KEYBOARDTYPE.BIG_ENG:
                currentType = KEYBOARDTYPE.SMALL_ENG;
                break;

            case KEYBOARDTYPE.SMALL_KOR:
                currentType = KEYBOARDTYPE.BIG_KOR;
                break;

            case KEYBOARDTYPE.BIG_KOR:
                currentType = KEYBOARDTYPE.SMALL_KOR;
                break;
        }

        KeyboardType[currentType].SetActive(true);
    }

    // 한글자 지우기
    public void Push_Back()
    {
        try
        {
            // 마지막 자리를 지우는 작업
            var index = input.text.Length - 1;
            var text = input.text;
            text = text.Substring(0, index);

            input.text = text;
        }
        catch
        {
            print("Text Empty");
        }
    }

    // 공백
    public void Push_Space()
    {
        input.text += " ";
    }

    // 한,영 변경
    public void Push_Change()
    {
        KeyboardType[currentType].SetActive(false);

        // 현재 상태가 영어라면 한글로 변경
        if (currentType == KEYBOARDTYPE.SMALL_ENG || currentType == KEYBOARDTYPE.BIG_ENG)
            currentType = KEYBOARDTYPE.SMALL_KOR;
        // 한글이라면 영어로 변경
        else
            currentType = KEYBOARDTYPE.SMALL_ENG;

        KeyboardType[currentType].SetActive(true);
    }

    float prev_height = 0f;

    // 입력한 문자열 출력
    public void Push_Enter()
    {
        // 아무 값도 없으면 바로 종료
        if (input.text == string.Empty) return;

        // Text 객체 생성
        var text = GameObject.Instantiate(m_TextProtoType) as Text;
        text.gameObject.SetActive(true);

        // 부모 오브젝트 갱신
        text.transform.SetParent(m_Content.transform);

        // 위치값 초기화
        text.rectTransform.anchoredPosition = new Vector2(0, (-10 - prev_height));
        text.rectTransform.localScale = Vector3.one;

        // Text 값 갱신
        text.text = input.text;

        // InputField 값 초기화
        input.text = string.Empty;

        // 위기값 동기화
        StartCoroutine(DelaySizeUpdate());
    }

    // 오브젝트 생성 후 위치값을 알아내기 위해 타이밍을 맞추기 위한 코루틴
    IEnumerator DelaySizeUpdate()
    {
        yield return new WaitForSeconds(0.01f);
        // 마지막으로 생성된 오브젝트의 height 값
        var height = m_Content.GetChild(m_Content.childCount - 1).GetComponent<RectTransform>().sizeDelta.y;

        // ProtoType 의 height 보다 작은 값이라면 값 수정
        if (height < m_TextProtoType.rectTransform.sizeDelta.y)
            height = m_TextProtoType.rectTransform.sizeDelta.y;

        // 스크롤 뷰 Content 사이즈 변경
        m_Content.sizeDelta = new Vector2(m_Content.sizeDelta.x, (m_Content.sizeDelta.y + height));

        // 이전 위치 값 갱신
        prev_height += height;
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
            Application.Quit();
    }
}
