using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ChangeScene : MonoBehaviour
{
    public string newScene;


    void OnTriggerEnter(Collider other)
    {
        if(other.CompareTag("Remy"))
        {
            SceneManager.LoadScene(newScene);
        }
    }

    public void playGame()
    {
        SceneManager.LoadScene("AncientRuinsScene");
    }

    public void mainMenu()
    {
        SceneManager.LoadScene("MainMenuScene");
    }

    public void quitGame()
    {
        Application.Quit();
    }
}
