using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

// Json 형식 저장을 위한 클래스 데이터
[Serializable]
public class LineDataPack
{
    public Vector3[] positions;
    public float width;
    public Color color;

    public LineDataPack(int ArrayLength)
    {
        positions = new Vector3[ArrayLength];
        width = 0f;
        color = Color.black;
    }
}

public class Drawing : MonoBehaviour
{
    [Header("Steam VR Objects")] public GameObject m_VRPlayer;

    [Header("VR Hand")] public Transform m_VRHandPosition;
    [Header("Non VR Hand(FallbackHand)")] public Transform m_NonVRHandPosition;
    [Header("LineRenderer Origin")] public LineRenderer m_LineRender_Origin;

    [Header("Select Drawing Layer")] public LayerMask m_DrawingLayer;
    [Header("Check Sphere Radius")] public float m_radius;

    [Header("BrushSize Text")] public TextMeshPro m_BrushSizeText;
    [Header("Redo Stack Object")] public Transform m_RedoStackObject;
    [Header("Brush Sample Object")] public LineRenderer m_BrushSampleObject;

    LineRenderer LineRender;
    Transform DrawLine;
    bool Pressed = false;

    // 중복 위치 제거를 위환 리스트
    List<Vector3> LineCheckArray = new List<Vector3>();

    // 실행 취소, 뒤로 기능 구현을 위한 스택
    Stack<GameObject> DrawStack = new Stack<GameObject>();
    Stack<GameObject> RedoStack = new Stack<GameObject>();
    Stack<int> RedoCount = new Stack<int>();

    // 현재 브러쉬의 상태를 알기 위한 벡터 배열과 스택
    Vector3[] CurrnetColorObjectPositions = new Vector3[2];
    Stack<GameObject> CurrentColorStack = new Stack<GameObject>();

    private void Start()
    {
        // default
        m_LineRender_Origin.startWidth = 0.05f;
        m_LineRender_Origin.endWidth = m_LineRender_Origin.startWidth;

        m_LineRender_Origin.startColor = Color.black;
        m_LineRender_Origin.endColor = m_LineRender_Origin.startColor;
        // ---------

        // VR 기기의 유무를 판단하여 컨트롤러 선택
        if (m_VRPlayer.activeInHierarchy)
            DrawLine = m_VRHandPosition;
        else
            DrawLine = m_NonVRHandPosition;

        // 현재 브러쉬 크기
        m_BrushSizeText.text = string.Format("{0:0.00}", m_LineRender_Origin.startWidth);

        // 브러쉬 미리보기 위치
        CurrnetColorObjectPositions[0] = new Vector3(2.42f, 0.95f, 5.7f);
        CurrnetColorObjectPositions[1] = new Vector3(2.31f, 0.95f, 5.7f);

        // 브러쉬 미리보기 라인 렌더러 생성
        m_BrushSampleObject.positionCount = 2;
        m_BrushSampleObject.SetPositions(CurrnetColorObjectPositions);

        // 브러쉬 미리보기 업데이트
        BrushSampleUpdate();
    }

    // 브러쉬 미리보기 업데이트
    void BrushSampleUpdate()
    {
        m_BrushSampleObject.startColor = m_LineRender_Origin.startColor;
        m_BrushSampleObject.endColor = m_BrushSampleObject.startColor;

        m_BrushSampleObject.startWidth = m_LineRender_Origin.startWidth;
        m_BrushSampleObject.endWidth = m_BrushSampleObject.startWidth;
    }

    // 그릴 라인 렌더러 생성
    LineRenderer InstanceDraw()
    {        
        // 라인 렌더러 프로토타입과 같은 속성의 객체 생성
        var obj = GameObject.Instantiate(m_LineRender_Origin.gameObject) as GameObject;
        obj.transform.SetParent(transform);
        // 스택에 푸쉬
        DrawStack.Push(obj);

        return obj.GetComponent<LineRenderer>();
    }

    // 라인 그리기
    void DrawingLine()
    {
        if (Input.GetMouseButtonDown(0))
        {
            LineRender = InstanceDraw();
            Pressed = true;
        }
        else if (Input.GetMouseButton(0))
        {
            if (Pressed)
            {
                // 현재 위치가 전에 그렸던 위치와 일치하지 않은 지 체크
                if (!LineCheckArray.Contains(DrawLine.position))
                {
                    // 라인 렌더러를 그리고, 현재 위치를 저장
                    LineRender.SetPosition(LineRender.positionCount++, DrawLine.position);                    
                    LineCheckArray.Add(DrawLine.position);
                }
            }
            else
            {
                LineRender = InstanceDraw();
                Pressed = true;
            }
        }
        else if (Input.GetMouseButtonUp(0))
        {
            Pressed = false;
            LineCheckArray.Clear();
        }
    }

    // 브러쉬 사이즈 키우기
    void BrushSizeUp()
    {
        m_LineRender_Origin.startWidth += 0.01f;
        if (m_LineRender_Origin.startWidth > 0.1f)
            m_LineRender_Origin.startWidth = 0.1f;
        m_LineRender_Origin.endWidth = m_LineRender_Origin.startWidth;

        m_BrushSizeText.text = string.Format("{0:0.00}", m_LineRender_Origin.startWidth);

        BrushSampleUpdate();
    }

    // 브러쉬 사이즈 줄이기
    void BrushSizeDown()
    {
        m_LineRender_Origin.startWidth -= 0.01f;
        if (m_LineRender_Origin.startWidth < 0.01f)
            m_LineRender_Origin.startWidth = 0.01f;

        m_LineRender_Origin.endWidth = m_LineRender_Origin.startWidth;

        m_BrushSizeText.text = string.Format("{0:0.00}", m_LineRender_Origin.startWidth);

        BrushSampleUpdate();
    }

    // 마지막으로 그렸던 라인렌더러 지우기
    void BackDrawing()
    {
        if (DrawStack.Count == 0) return;

        // 마지막으로 그린 라인 정보 갱신
        var obj = DrawStack.Pop();

        // 실행 취소 오브젝트 하위로 이동
        obj.transform.SetParent(m_RedoStackObject);

        // 비활성화
        obj.SetActive(false);

        // 실행 취소를 위한 스택에 푸쉬
        RedoStack.Push(obj);

        // 몇 번 지웠는가 에 대한 카운터를 위한 스택 ( 한 번 클릭당 한 번만 지우기 때문에 1 푸쉬 )
        RedoCount.Push(1);
    }

    // 실행 취소
    void RedoDrawing()
    {
        if (RedoStack.Count == 0) return;

        // 한 번에 몇 개가 지워졌는 지 확인
        int length = RedoCount.Pop();

        // 한 번에 지워진 개수 만큼 다시 실행 취소
        for (int i = 0; i < length; i++)
        {
            // 스택 내에 있는 데이터 갱신
            var obj = RedoStack.Pop();
            
            // 다시 드로우 오브젝트 하위로 이동
            obj.transform.SetParent(transform);

            // 활성화
            obj.SetActive(true);

            // 드로우 스택에 푸쉬
            DrawStack.Push(obj);
        }
    }

    // 전체 지우기
    void ResetDrawing()
    {   
        // 현재 그려진 라인의 전체 수 저장
        int length = DrawStack.Count;

        // 그려진 라인의 전체 수만큼 지우기
        for (int i = 0; i < length; i++)
        {
            var obj = DrawStack.Pop();
            obj.transform.SetParent(m_RedoStackObject);
            obj.SetActive(false);

            RedoStack.Push(obj);
        }

        // 전체 수만큼 지워졌기 때문에 그만큼의 크기를 푸쉬
        // 실행 취소를 누르면 한 번에 지워졌던 라인들이 다시 한 번에 활성화하게 됨
        RedoCount.Push(length);
    }

    // 브러쉬 컬러 선택 후변경
    IEnumerator ColorSetting()
    {
        // 화면 전체를 스크린샷으로 찍어 텍스처 파일로 만듬.
        // 현재 마우스 위치에 있는 색상의 RGB 값을 뽑아냄.
        // 뽑아 낸 색상을 브러쉬 색상에 적용

        // 스크린샷을 찍을 때 컨트롤러는 찍히면 안되므로 잠시 비활성화
        DrawLine.gameObject.SetActive(false);

        // 스크린샷을 찍어 텍스처 파일로 저장
        Texture2D tex = new Texture2D(Screen.width, Screen.height, TextureFormat.RGB24, false);
        yield return new WaitForEndOfFrame();
        tex.ReadPixels(new Rect(0, 0, Screen.width, Screen.height), 0, 0);
        tex.Apply();

        // 스크린샷을 다 찍었으니 다시 컨트롤러 활성화
        DrawLine.gameObject.SetActive(true);

        // 현재 컨트롤러 위치를 월드 포지션으로 변경
        var pos = Camera.main.WorldToScreenPoint(DrawLine.position);
        // 변경된 컨트롤러의 월드 포지션에 위치한 색상 뽑기
        Color color = tex.GetPixel((int)pos.x, (int)pos.y);

        // 브러쉬 색상 변경
        m_LineRender_Origin.startColor = color;
        m_LineRender_Origin.endColor = m_LineRender_Origin.startColor;

        BrushSampleUpdate();
    }

    // 데이터 저장
    void DataSave()
    {
        // 현재까지 그린 라인 정보 배열 갱신
        var obj = DrawStack.ToArray();

        // Json 에 저장할 리스트
        List<LineDataPack> packlist = new List<LineDataPack>();

        for (int i = 0; i < obj.Length; i++)
        {
            // 라인렌더러 컴포넌트 정보 갱신
            var data = obj[i].GetComponent<LineRenderer>();

            // Json 에 저장할 데이터 패키지 생성
            LineDataPack pack = new LineDataPack(data.positionCount);

            // 각 정보들을 채워 넣음
            // 브러쉬 위치값, 브러쉬 크기, 브러쉬 색상
            data.GetPositions(pack.positions);
            pack.width = data.startWidth;
            pack.color = data.startColor;

            // 리스트에 추가
            packlist.Add(pack);
        }

        // 리스트 내의 정보를 Json 형식으로 포맷
        string packTojson = JsonHelper.ToJson(packlist.ToArray());

        // PlayerPrefs 을 통해 레지스트리에 string 저장
        PlayerPrefs.SetString("LineDataToJson", packTojson);
    }

    // 스택 내에 게임 오브젝트 전체 삭제
    void ObjectAllDestory(Stack<GameObject> stack)
    {
        int length = stack.Count;
        for (int i = 0; i < length; i++)
            DestroyImmediate(stack.Pop());
    }

    // 데이터 로드
    void DataLoad()
    {
        try
        {
            // 현재 드로우, 실행취소 스택 내에 있는 모든 정보 삭제
            ObjectAllDestory(DrawStack);
            ObjectAllDestory(RedoStack);

            // PlayerPrefs 레지스트리에 저장되어 있는 json 형식의 string 값을 불러와서
            // LineDataPack 형식으로 파싱
            var json = JsonHelper.FromJson<LineDataPack>(PlayerPrefs.GetString("LineDataToJson"));
            
            for (int i = 0; i < json.Length; i++)
            {
                // 라인 렌더러 생성
                var obj = InstanceDraw();

                // 각각 라인 렌더러 정보를 파싱된 Json 정보로 갱신
                // 결과적으로 저장된 라인 렌더러 정보를 다시 불러오게 됨
                obj.startWidth = json[i].width;
                obj.endWidth = obj.startWidth;

                obj.startColor = json[i].color;
                obj.endColor = obj.startColor;

                obj.positionCount = json[i].positions.Length;
                obj.SetPositions(json[i].positions);
            }
        }
        catch
        {
            // 예외처리
            print("Data Empty");
        }
    }

    // 클릭한 오브젝트의 태그에 따라 각각의 이벤트 실행
    void ClickEvent(string tag)
    {
        if (Input.GetMouseButtonDown(0))
        {
            switch (tag)
            {
                case "BrushSizeUp":
                    BrushSizeUp();
                    break;

                case "BrushSizeDown":
                    BrushSizeDown();
                    break;

                case "Back":
                    BackDrawing();
                    break;

                case "Redo":
                    RedoDrawing();
                    break;

                case "Reset":
                    ResetDrawing();
                    break;

                case "Color":
                    StartCoroutine(ColorSetting());
                    break;

                case "Save":
                    DataSave();
                    break;

                case "Load":
                    DataLoad();
                    break;
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (Physics.CheckSphere(DrawLine.position, m_radius, m_DrawingLayer))
        {
            DrawingLine();
        }
        else
        {
            var obj = Physics.OverlapSphere(DrawLine.position, m_radius);
            for (int i = 0; i < obj.Length; i++)
            {
                ClickEvent(obj[i].gameObject.tag);
                break;
            }
        }
        
        if (Input.GetKeyDown(KeyCode.Escape))
            Application.Quit();
    }
}
