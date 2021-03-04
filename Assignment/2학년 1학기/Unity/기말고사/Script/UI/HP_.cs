using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class HP_ : MonoBehaviour
{

    public GameObject player;
    private PlayerState state;

    private void Start()
    {
        state = player.GetComponent<PlayerState>();
    }


    void Update()
    {
        this.GetComponent<Image>().fillAmount = (float)((float)state.hp / (float)state.Max_hp);
    }
}
