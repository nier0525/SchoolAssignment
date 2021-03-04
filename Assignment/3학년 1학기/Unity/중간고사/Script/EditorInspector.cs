using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(SkillTreeEditorManager))]
public class EditorInspector : Editor
{
    SkillTreeEditorManager generator;
    static int index = 0;
    string[] data_list;

    private void OnEnable()
    {
        generator = (SkillTreeEditorManager)target;
        generator.InitSkillInfo();
    }

    void LayOutSetting()
    {
        SKillData data = generator.GetData();

        EditorGUILayout.Space();

        EditorGUILayout.LabelField(data.m_Name + " 스킬 정보");
        EditorGUILayout.Space();

        data.m_Name = EditorGUILayout.TextField("스킬 이름", data.m_Name);
        EditorGUILayout.Space();

        data.m_Image = (Sprite)EditorGUILayout.ObjectField("스킬 이미지", data.m_Image, typeof(Sprite), true);
        EditorGUILayout.Space();

        data.m_AttackCount = EditorGUILayout.IntField("스킬 타수 (최대 10)", data.m_AttackCount);
        EditorGUILayout.Space();

        for (int i = 0; i < data.m_AttackCount; i++)
        {
            data.m_DamegeFormula[i] = EditorGUILayout.TextField(i + 1 + " 타 데미지 공식", data.m_DamegeFormula[i]);
        }
        EditorGUILayout.Space();

        data.m_FrontSkill = (GameObject)EditorGUILayout.ObjectField("선행 스킬", data.m_FrontSkill, typeof(GameObject), true);
        EditorGUILayout.Space();

        data.m_FrontSkillCount = EditorGUILayout.IntField("스킬 해제 조건 설정", data.m_FrontSkillCount);
        EditorGUILayout.Space();

        data.m_Position = (SKillPosition)EditorGUILayout.EnumPopup("스킬창 위치", data.m_Position);
        EditorGUILayout.Space();

        data.m_Anim = (Animator)EditorGUILayout.ObjectField("스킬 애니메이션", data.m_Anim, typeof(Animator), true);
        EditorGUILayout.Space();

        data.m_Sound = (AudioSource)EditorGUILayout.ObjectField("스킬 사운드", data.m_Sound, typeof(AudioSource), true);
        EditorGUILayout.Space();

        data.m_Active = EditorGUILayout.Toggle("스킬 활성화 상태", data.m_Active);
        EditorGUILayout.Space();

        data.m_SkillLevel = EditorGUILayout.IntField("스킬 레벨", data.m_SkillLevel);
        EditorGUILayout.Space();

        data.m_MaxSkillLevel = EditorGUILayout.IntField("스킬 최대 레벨 설정", data.m_MaxSkillLevel);
        EditorGUILayout.Space();

        EditorGUILayout.LabelField("스킬 설명");
        data.m_Info = EditorGUILayout.TextArea(data.m_Info);
        EditorGUILayout.Space();

        generator.SetData(data);
    }

    void PopUpSetting()
    {
        data_list = new string[generator.Length()];

        for (int i = 0; i < data_list.Length; i++)
        {
            data_list[i] = generator.GetData(i).m_Name;
        }

        EditorGUILayout.Space();
        if (data_list.Length != 0)
        {            
            index = EditorGUILayout.Popup(selectedIndex: index, displayedOptions: data_list);
            EditorGUILayout.Space();
        }
    }

    void ButtonSetting()
    {
        if (GUILayout.Button("스킬 만들기"))
        {
            generator.CreateSkill();
            generator.SetSkillInfo(generator.Length() - 1);
            index = generator.Length() - 1;
            generator.InitSkillInfo();
        }
        EditorGUILayout.Space();

        if (GUILayout.Button("스킬 정보 수정"))
        {
            generator.SetSkillInfo(index);
            generator.InitSkillInfo();
            generator.SelectSkill(index);
        }
        EditorGUILayout.Space();

        if (GUILayout.Button("스킬 정보 새로고침"))
        {
            generator.InitSkillInfo();
            generator.SelectSkill(index);
        }
        EditorGUILayout.Space();

        if (GUILayout.Button("스킬 삭제"))
        {
            if (data_list[index] == generator.GetData().m_Name)
            {
                generator.RemoveSkill(index);
                generator.InitSkillInfo();
            }
            else
            {
                generator.InitSkillInfo();
                generator.SelectSkill(index);
            }
        }
    }

    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();

        generator = (SkillTreeEditorManager)target;

        // 요소
        if (generator.Length() != 0 && generator.Length() > index) LayOutSetting();

        // 팝업
        PopUpSetting();

        // 버튼
        ButtonSetting();
    }
}
