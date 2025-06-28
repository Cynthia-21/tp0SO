package ar.edu.utn.frba.dds.servicioagregador;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@SpringBootApplication
public class ServicioAgregadorApplication {

  public static void main(String[] args) {
    SpringApplication.run(ServicioAgregadorApplication.class, args);
  }

}
