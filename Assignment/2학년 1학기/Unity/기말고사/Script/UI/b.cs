using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class b : MonoBehaviour
{
    public GameObject of1,on,of2,of3,on2,on3;
    public void S2Click()
    {
        Debug.Log("S2");
        of1.SetActive(false);
        of2.SetActive(false);
        of3.SetActive(false);
        on.SetActive(true);
        on2.SetActive(true);
        on3.SetActive(true);
    }
}
