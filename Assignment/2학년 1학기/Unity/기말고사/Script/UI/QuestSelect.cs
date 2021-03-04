using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class QuestSelect : MonoBehaviour
{
    public GameObject Q1, Q2, Q3;
    public Sprite Sel, NotSel;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Quest.Q <= 2)
        {
            Q1.GetComponent<Image>().sprite = Sel;
        }

        else if (Quest.Q <= 5)
        {
            Q1.GetComponent<Image>().sprite = NotSel;
            Q2.GetComponent<Image>().sprite = Sel;
        }

        else
        {
            Q2.GetComponent<Image>().sprite = NotSel;
            Q3.GetComponent<Image>().sprite = Sel;
        }
    }
}
