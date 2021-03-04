using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Point : MonoBehaviour
{
    public GameObject obj, obj2;
    public GameObject moon;
    public GameObject ui, ui2, ui3, ui4, ui5, ui6;

    static int score = 0;

    private void Start()
    {
        obj.SetActive(false);
        obj2.SetActive(false);

        ui.SetActive(true);
        ui2.SetActive(true);
        ui3.SetActive(true);
        ui4.SetActive(true);
        ui5.SetActive(true);
        ui6.SetActive(true);

        score = 6;
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.name == "unitychan")
        {
            score -= 1;
            GameObject.Find("Score").GetComponent<AudioSource>().Play();
            Destroy(gameObject);

            switch (score)
            {
                case 0:
                    moon.SetActive(false);
                    ui6.SetActive(false);
                    break;
                case 1:
                    ui5.SetActive(false);
                    break;
                case 2:
                    obj.SetActive(true);
                    ui4.SetActive(false);
                    break;
                case 3:
                    ui3.SetActive(false);               
                    break;
                case 4:
                    obj2.SetActive(true);
                    ui2.SetActive(false);
                    break;
                case 5:

                    ui.SetActive(false);
                    break;
            }
        }
    }

    //private void OnGUI()
    //{
    //    GUI.Box(new Rect(Screen.width - 130, 10, 120, 30), " ");
    //    GUI.Label(new Rect(Screen.width - 110, 13, 108, 27), "남은 결계 : " + score.ToString());
    //}

    private void Update()
    {

    }
}