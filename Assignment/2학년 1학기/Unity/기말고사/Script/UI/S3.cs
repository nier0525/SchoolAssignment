using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class S3 : MonoBehaviour
{
    public GameObject _S1, _S2, _S3;


    void OnMouseDown()
    {
        GameObject.Find("Vol").GetComponent<AudioSource>().Play();

        _S1.SetActive(false);
        _S2.SetActive(false);
        _S3.SetActive(true);
    }

}
