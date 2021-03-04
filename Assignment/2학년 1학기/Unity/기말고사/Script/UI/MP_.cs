using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MP_ : MonoBehaviour
{
    // Start is called before the first frame update
    public GameObject player;
    private PlayerState state;

    private void Start()
    {
        state = player.GetComponent<PlayerState>();
    }


    void Update()
    {
        this.GetComponent<Image>().fillAmount = (float)state.mp/ (float)state.Max_mp ;
    }
}
