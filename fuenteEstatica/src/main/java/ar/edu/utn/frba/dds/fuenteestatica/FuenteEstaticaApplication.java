package ar.edu.utn.frba.dds.fuenteestatica;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@SpringBootApplication
@RestController
public class FuenteEstaticaApplication {

  @GetMapping("/fuenteEstatica/HealthCheck")
  public String healthCheck() {
    return "OK";
  }

  public static void main(String[] args) {
    SpringApplication.run(FuenteEstaticaApplication.class, args);
  }

}
