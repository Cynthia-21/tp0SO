package ar.edu.utn.frba.dds.servicioagregador.models.domain;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.time.LocalDate;
import java.time.Period;

@AllArgsConstructor
@NoArgsConstructor
@Builder
@Setter
@Getter
public class Contribuyente {
  private String nombre;
  private String apellido;
  private LocalDate fechaNacimiento;

  public Contribuyente(String nombre, String apellido, Integer edad) {
    this.nombre = nombre;
    this.apellido = apellido;
    this.fechaNacimiento = fechaNacimiento;
  }

  public Integer edad() {
    return Period.between(this.fechaNacimiento, LocalDate.now()).getYears();
  }
}
