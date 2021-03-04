using UnityEngine;
using UnityEngine.UI;

public class KeyBoardValueSetting : MonoBehaviour
{
    // 버튼 텍스트 오브젝트 이름으로 자동 갱신 기능
    [ContextMenu("Text Update")]
    void UpdateText()
    {
        var Text = GetComponentInChildren<Text>();
        Text.text = gameObject.name;
    }
}
